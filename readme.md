This is the source code for a tutorial on [my website]. The tutorial is 
broken into three parts. 

- [Part One]: Introduction, Architecture, Internal C Function 
- [Part Two]: Using qdecoder, creating the Web facing API (not released yet)
- Part Three: Creating the Web Interface to the Chat Server (not released yet)

Also, I will be tagging the ending commits of each tutorial so it is easy to 
look through the commit timeline and view the project at different points in time.

Once you've cloned the repository you can use

    git tag -l

to view the list of tags and then `checkout tags/<name>` to switch to that tag.

As described in [Part Two], you'll need to setup your Apache configuration to 
something like this to run the application:

	<VirtualHost *:80>
            ServerAdmin webmaster@localhost
            ServerName www.chat.dev
            DocumentRoot /path/to/this/repository/tutorialchat/www
            <Directory />
                    Options Indexes
                    AllowOverride None
            </Directory>
            Alias /chat /path/to/this/repository/tutorialchat/bin
            <Directory />
                    AddHandler cgi-script .cgi
                    AllowOverride None
                    Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
                    Order allow,deny
                    Allow from all
            </Directory>
    
            ErrorLog /path/to/this/repository/tutorialchat/error.log
    
            # Possible values include: debug, info, notice, warn, error, crit,
            # alert, emerg.
            LogLevel warn
    
    </VirtualHost>

[my website]:http://www.ethanjoachimeldridge.info
[Part One]:http://www.ethanjoachimeldridge.info/tech-blog/cgi-c-harp-1
