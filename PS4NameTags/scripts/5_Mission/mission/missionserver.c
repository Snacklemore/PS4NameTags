modded class MissionServer
{
	float   g_NameTagDistance ;
	vector  g_NameTagAnchor; 
	
	ref ConfigTransmit1 m_ConfigRPC;
	
	
	void MissionServer()
	{
		
		m_ConfigRPC = new ref ConfigTransmit1("RPConfig","OnConfigTransmitRPCNameTag");
		Print( "Loaded Server Mission" );
		
		
	}
	
};