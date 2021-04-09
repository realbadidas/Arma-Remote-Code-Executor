# Arma-Remote-Code-Executor
With this servermod you can easily send script commands via a POST request with HTTP or HTTPS to your ArmA server! (Maybe even your DayZ server i have no clue lol test it yourself)


# Hello there!
This is an robust and fast ArmA 3 Servermod that runs on 32 bit and 64 bit Windows (Sooner or later also Linux but i have no machine to test or compile)! It listens on a specified port for incoming requests. The servermod consists of a pbo and 6 DLLs (2 ArmA extension DLLs and 4 library DLLs for OpenSSL).


# How to install
Download the .ZIP file in the <a href=https://github.com/realbadidas/Arma-Remote-Code-Executor/releases>releases</a> tab. Now its just drag and drop: Drag the contents of the folder "A3" into your ArmA 3 Dedicated Server folder, start your server with the mod loaded and it should work right out of the box! But please, for christs sake, change the default username and password:p


# Config File
The Config File is pretty self explanatory
<img src=https://i.imgur.com/wZOOfdv.png></img>

<b>Fields:</b>
host (string, default: "127.0.0.1"): A hostname (or IP-Address) <br />
port (string, default: "6601"): Port <br /><br />

https (boolean, default: true): If you want to use HTTPS or not <br /><br />

SSL_Key (string, default: "@sqfremoteexecute\ssl.key"): Path to your ssl.key file (only needed if using https) <br />
SSL_Cert (string, default: "@sqfremoteexecute\ssl.cert"): Path to your ssl.cert file (only needed if using https) <br /><br />

users (array that contains an array of credentials, default user is "foo" with pass of "bar", <b>1337 is the maximum number of user accounts!</b> <br /><br />


# Safety Reminder
Always use HTTPS!!! If you are not using it, every time you use this tool (except you use localhost or your local connection), a hacker can just grab the username and password and do bad things afterwards.
