diag_log "ok";
if (isNil 'sqfexecute_server_isOn') then {
	sqfexecute_server_isOn = false;

	_res = "sqfexecute" callExtension "0";
	diag_log format["Plain: %1", _res];
	
	
	if (_res != "OK") exitWith {
			diag_log format["Extension sent nothing good - The addon won't work! Aborting..."];
	};
	
	diag_log format["Extension sent OK | %1", _res];
	_res = "sqfexecute" callExtension "2";
	
	if (_res == "OK") then {
		sqfexecute_server_isOn = true;
		[[], "thr"] call BIS_fnc_spawnOrdered;
	} else {
	sqfexecute_server_isOn = false;
	};
};

thr = {
	while {true} do {
		_res = "sqfexecute" callExtension "5";
		if(_res != "no-message-yet") then {
			diag_log format["New execution! | RES: %1", _res];
			
			_retam = 0 call compile _res;
			
			"sqfexecute" callExtension format["4%1", _retam];
		};
		uiSleep 0.03;
	};
};