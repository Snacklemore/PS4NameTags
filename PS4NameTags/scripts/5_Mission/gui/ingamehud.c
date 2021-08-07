

class ConfigTransmit1 
{
	vector g_NameTagAnchor ;
	float  g_NameTagDistance;
	int  	COMBAT_MODE_TIMER;
	float	COMBAT_FLAG_DISTANCE;
	
	void ConfigTransmit1(string namespace,string  funcName)
	{
		GetRPCManager().AddRPC(namespace, funcName, this, SingleplayerExecutionType.Both);
	}
	
	void OnConfigTransmitRPCNameTag(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2<vector,float> data;
		if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Server )
        {
            Print( "Server function called!" );
        }
        else
        {
			
			
			
			
				Print( "[NameTag::RPC]:: Client function called!" );
				vector anchor  = data.param1;
				float dist = data.param2;
			
				g_NameTagAnchor = anchor;
				g_NameTagDistance =  dist;
				Print( "anchor "+  anchor );
				Print ("dist: " + dist);
			
            
        }
	}
	
	

};





modded class IngameHud
{
	ref ConfigTransmit1 m_ConfigRPC;
	override void Init(Widget hud_panel_widget )
	{
		super.Init(hud_panel_widget);
		m_ConfigRPC = new ref ConfigTransmit1("RPCConfig","OnConfigTransmitRPCNameTag");
	
	}
	
//Note: written by BI,  just removed #ifdef PS4, and a  distance  check
//PS4 NameTags, add  a  distance check to trader town and we are good
//BI is using  raycast  to tag  player  currently  in  crosshair
void RefreshPlayerTags()
	{
		
		if( GetGame().GetPlayer() )
		{
			

			//print data transmitted in OnClientReadyEvent on server 
			float distance_n = vector.Distance(m_ConfigRPC.g_NameTagAnchor,GetGame().GetPlayer().GetPosition());
			if(distance_n > 20)
				return;
			Print("Distance TO Anchor: " + distance_n);
			Print("Max Dist: " + m_ConfigRPC.g_NameTagDistance);
			
			if(distance_n > 20)
				return;
			bool found = false;
			vector head_pos = GetGame().GetCurrentCameraPosition();
			float distance;
			foreach( Man player : ClientData.m_PlayerBaseList )
			{
				vector target_player = player.GetPosition();
				distance = vector.Distance( head_pos, target_player );
				
				target_player[1] = target_player[1] + 1.2;
				
				if( distance <= 15 && player != GetGame().GetPlayer() )
				{
					vector screen_pos = GetGame().GetScreenPosRelative( target_player );
					vector end_pos = head_pos + GetGame().GetCurrentCameraDirection() * 25;
					RaycastRVParams params = new RaycastRVParams( head_pos, end_pos, GetGame().GetPlayer(), 0 );
					params.sorted = true;
					
					array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
					DayZPhysics.RaycastRVProxy( params, results );
					if( results.Count() > 0 )
					{
						if( results.Get( 0 ).obj == player )
						{
							m_CurrentTaggedPlayer = PlayerBase.Cast( player );
							Print("Tagged Player: "  +  player.GetIdentity().GetName());
							found = true;
						}
					}
				}
			}
			if( !found )
			{
				m_CurrentTaggedPlayer = null;
			}
		}
	}
	
	
	void ShowPlayerTag( float timeslice )
	{
		if( m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() )
		{
			
			
			if( !m_PlayerTag )
			{
				m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("gui/layouts/new_ui/hud/hud_player_tag.layout");
				m_PlayerTagText = TextWidget.Cast( m_PlayerTag.FindAnyWidget( "TagText" ) );
				//m_PlayerTagText.SetText( m_CurrentTaggedPlayer.GetIdentity().GetName() );
			}
			m_PlayerSpineIndex = m_CurrentTaggedPlayer.GetBoneIndex( "Spine2" );
			vector player_pos = m_CurrentTaggedPlayer.GetBonePositionWS( m_PlayerSpineIndex );
			vector screen_pos = GetGame().GetScreenPosRelative( player_pos );
			
			if( screen_pos[2] > 0 )
			{
				if( screen_pos[0] > 0 && screen_pos[0] < 1 )
				{
					if( screen_pos[1] > 0 && screen_pos[1] < 1 )
					{
						m_PlayerTagText.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1 ) );
						m_PlayerTag.SetPos( 0.55, 0.55 );
						m_PlayerTagText.SetText( m_CurrentTaggedPlayer.GetIdentity().GetName() );
						
						//m_PlayerTagText.SetSize( 1, 1 - screen_pos[2] / 25  );
						return;
					}
				}
			}
		}
		
		if( m_PlayerTag )
		{
			float new_alpha = Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 );
			m_PlayerTagText.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 ) );
			if( new_alpha == 0 )
			{
				m_PlayerTagText.SetText( "" );
				m_CurrentTaggedPlayer = null;
			}
		}
	}
	
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );

		m_ActionTargetsCursor.Update();
		DisplayPresence();
		//
		//modifiers - tendency status (critical)
		if ( m_BlinkTime > TENDENCY_BLINK_TIME )
		{
			//get color & alpha
			if ( m_TendencyStatusCritical.Count() > 0 )
			{
				for ( int i = 0; i < m_TendencyStatusCritical.Count(); ++i )
				{
					
					ImageWidget widget = m_TendencyStatusCritical.GetKey( i );
					int alpha = widget.GetAlpha() * 255;
					int color;
					int colorA = ARGB( alpha, 255, 255, 255 );
					int colorB = m_TendencyStatusCritical.Get( widget );
					
					//set new color to different color
					if ( widget.GetColor() == colorA )
					{
						color = colorB;
					}
					else
					{
						color = colorA;
					}

					widget.SetColor( color );
				}
			}
			
			m_BlinkTime = 0;	//reset
		}
		else
		{
			m_BlinkTime += timeslice;
		}
		
		if( m_IsTemperatureVisible )
		{
			m_TemperatureTimer += timeslice;
			if( m_TemperatureTimer > m_TemperatureShowTime )
			{
				HideTemperature();
			}
		}
		
		if ( !m_IsStaminaVisible )
		{
			m_StaminaTimer += timeslice;
			if ( m_StaminaTimer > m_StaminaShowTime )
			{
				SetStaminaBarVisibility( true );
			}
			
		}
		
		RefreshVehicleHud( timeslice );
		
		
		
		RefreshPlayerTags();
		ShowPlayerTag(timeslice);
		
	}
};