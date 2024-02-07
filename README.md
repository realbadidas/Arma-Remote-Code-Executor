# Arma-Remote-Code-Executor
With this servermod you can easily send script commands or SQF code via a POST request with HTTP or HTTPS to your ArmA server!


# Hello there!
This is an robust and fast ArmA 3 Servermod that runs on 32 bit and 64 bit Windows (I don't think it will support linux anytime soon, it is too much work for me, i should have used CMake in the first place...) It listens on a specified port for incoming requests and returns a JSON-Array with a response code and the return of the executed script. The servermod consists of a pbo and 6 DLLs (2 ArmA extension DLLs and 4 library DLLs for OpenSSL).


This is still under development, so you may encounter bugs, please create an issue on the github page explaining your bug and it will get fixed ASAP!)
Inspired by <a href=https://github.com/dylanplecki/sqf-rcon>this</a>

# How to install
Download the .ZIP file in the <a href=https://github.com/realbadidas/Arma-Remote-Code-Executor/releases>releases</a> tab. Now its just drag and drop: Drag the contents of the folder "A3" into your ArmA 3 Dedicated Server folder, start your server with the mod loaded and it should work right out of the box! But please, for christs sake, change the default username and password :p

I would also recommend to use "-autoInit" as a startparameter so you can use it even if there are no players on your server


# How to use
Send a POST request to "{IP}:{PORT}/send" with the parameters "user", "pass" and "execute", where "user" represents the username, "pass" the password and "execute" the SQF Code you want to execute.

If you use HTTPS you need to get a SSL Certificate, you can just generate a self signed certificate from a website like <a href=https://www.selfsignedcertificate.com/>this</a>, add your domain/IP, download the .key and .cert file and paste it in the mod folder. Remember to edit the config.


# For Developers
This is built in VS2019 (vc-142) using the <a href=https://github.com/ipkn/crow>crow webserver</a>, the boost library from nuget (vc141, version 1.69 (all above 1.69 will cause compiler issues)) It also uses the openssl-vc142 package from nuget for https support for crow.


# Config File
The Config File is pretty self explanatory
<img src=https://i.imgur.com/8vqkkXS.png></img>


<b>Fields:</b><br />
host (string, default: "127.0.0.1"): A hostname (or IP-Address) <br />
port (string, default: "6601"): Port <br />
consoleWindow (boolean, default: "true"): If true, a console window will appear that outputs debugging information <br />
https (boolean, default: true): If you want to use HTTPS or not <br />
SSL_Key (string, default: "@sqfremoteexecute\ssl.key"): Path to your ssl.key file (only needed if using https) <br />
SSL_Cert (string, default: "@sqfremoteexecute\ssl.cert"): Path to your ssl.cert file (only needed if using https) <br />
users (array that contains an array of credentials, default user is "foo" with pass of "bar" (you need to hash the password with the SHA256 algorithm), <b>1337 is the maximum number of user accounts!^^</b>


# Safety Reminder
Always use HTTPS!!! If you are not using it, every time you use this tool (except you use localhost or your local connection), a hacker can just grab the username and password and do bad things afterwards.

# Known Issues
1. If you have a bad JSON-Config your server will crash.
2. If you have bad paths to your ssl certificate files your server will crash.
3. If you rename the mod the server may crash or the mod doesn't even start.

# Examples of things you can do with this
- Live Mapmarker on a website of players that are playing on the server.
- Live whitelist (give licences to people while they are playing on the server (this example is for altis life)).
- Give player weapons or some other things, like cars and stuff.
- Announce things like events just with a button on a website/admintool.
- Debugging purposes.
- And even more!
