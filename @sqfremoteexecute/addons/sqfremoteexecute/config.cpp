class sqfremoteexecute {
	name="SQF Remote Executor";
	dir="@sqfremoteexecute";
	version="1.0";
	author[]= {
		"badidas";
	};
	requiredAddons[]={};
	requiredVersion=1;
	units[]={};
	weapons[]={};
	hideName="false";
	hidePicture="true";
	description="A remote code executor for scripting outside the server!";
};

class CfgPatches {
	class sqfexecute_main: sqfremoteexecute { };
};

class CfgMods {
	class sqfexecute_mod: sqfremoteexecute { };
};

class CfgFunctions {
	class sqfremoteexecute {
		class main {
			class startInit {
				file="sqfremoteexecute\fnc_startInit.sqf";
				preInit=1;
			};
		};
	};
};

class Extended_PreInit_EventHandlers {
	sqfremoteexecute="call compile preprocessFileLineNumbers 'sqfremoteexecute\fnc_startInit.sqf'";
};
