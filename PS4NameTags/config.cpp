class CfgPatches
{
 class PS4NameTags
 {
  requiredAddons[]=
  {
    JM_CF_Scripts
  };
 }
}
class CfgMods
{
 class PS4NameTags
 {
  type = "mod";
  dependencies[]=
		{
			"Game",
			"Mission",
			"gui"
		};
  
  class defs
  {
   
   class missionScriptModule
			{
				value  = "";
				files[]=
				{
					"PS4NameTags/scripts/5_Mission"
				};
			};
	class gameScriptModule
			{
				value  = "";
				files[]=
				{
					"PS4NameTags/AntiCombatLog/scripts/3_Game"
				};
			};
   
  }
  
 }
 
}