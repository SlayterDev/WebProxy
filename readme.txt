Group 5 - Web Proxy
===================

Bradley Slayter
Gaga Tsungi
Chris Wolfe


Compiling & Setup
=================
1. Run 'make' in the project directory to compile the source code
2. Create a directory called 'tmp' in the directory of the executable to store the cache files


Running
=======
Use './Proxy <port>' where <port> is the number of the port you wish to run the proxy on. Connect to the proxy from a web browser using 'http://<server address>:<port>/www.cse.unt.edu'. You may replace the website after the slash with any website you'd like to visit.

Blacklists:
To use a black list to block certain sites, create a file called 'blacklist.txt'. Enter each site you wish to block on a single line.
