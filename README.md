# PS4NameTags
NameTags  for DayZ, as they are used on PlayStation4
-Configurable Centerpoint, or  Anchor, to  meaure player distance from.
-Configurable distance to stop nametag from showing up.

#Setting Up
For the NameTags to work properly you need to assign a few variables inside you init.c on your server.

```java
class CustomMission: MissionServer
{
	
	
	void  CustomMission()
	{
		//NameTag Center
		g_NameTagAnchor = "1337.7  1337.7  1337.7";
		g_NameTagDistance = 69.0;
	}
	
	
	
	
	override void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientReadyEvent(identity,player);
		GetRPCManager().SendRPC("RPCConfig","OnConfigTransmitRPCNameTag",new Param2<vector,float>(g_NameTagAnchor,g_NameTagDistance),false,identity);
	}	
```

Dependant on Community Framework by JacobMango
