// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#include <grace-coreapi/module.h>
#include <grace/filesystem.h>

#ifdef __FLAVOR_LINUX_DEBIAN
	#define SVCNAME "apache2"
	#define CONFDIR "/etc/apache2/openpanel.d"
#else
	#define SVCNAME "httpd"
	#define CONFDIR "/etc/httpd/openpanel.d"
#endif

// ==========================================================================
// CLASS HTTPForward -> Domain:HTTPForward
// ==========================================================================
class HTTPForward : public CoreClass
{
public:
	HTTPForward (void) : CoreClass ("Domain:HTTPForward")
	{
	}
	
	~HTTPForward (void)
	{
	}
	
	// ----------------------------------------------------------------------
	// CREATE
	// ----------------------------------------------------------------------
	bool create (void)
	{
		return update ();
	}
	
	// ----------------------------------------------------------------------
	// UPDATE
	// ----------------------------------------------------------------------
	bool update (void)
	{
		file f;
		string fname = "%s.conf" %format (id);
		
		if (! f.openwrite (fname))
		{
			error (CoreModule::E_OTHER, "Could not write file");
			return false;
		}
		
		try
		{
	        f.writeln ("################################################################################");
	        f.writeln ("## WARNING: AUTOMATICALLY GENERATED                                           ##");
	        f.writeln ("## This file was automatically generated by OpenPanel. Manual changes to this ##");
	        f.writeln ("## file will be lost the next time this file is generated.                    ##");
	        f.writeln ("################################################################################");
	        f.writeln ("\n");

			f.writeln ("<VirtualHost *:80>");
			f.writeln ("  ServerAdmin webmaster@%s" %format (id));
			
			if (param["type"] != "frame")
			{
				f.writeln ("  DocumentRoot /var/www/forwards/%s" %format (id));
			}
			
			f.writeln ("  ServerName %s" %format (id));
			
			if (id.strncmp ("www.", 4) == 0)
			{
				f.writeln ("  ServerAlias %s" %format (id.mid(4)));
			}
			
			value aliases = listAliases (env);
			foreach (alias, aliases)
			{
				f.writeln ("  ServerAlias %s" %format (alias));
				if (alias.strncmp ("www.", 4) == 0)
				{
					f.writeln ("  ServerAlias %s" %format (alias.mid(4)));
				}
			}
			
			if (param["type"] == "redirect")
			{
				f.writeln ("  Redirect / %s" %format (param["target"]));
			}
			f.writeln ("</VirtualHost>");
			f.close ();
		}
		catch (exception e)
		{
			error (CoreModule::E_OTHER, "File write exception: %s"
												%format (e.description));
			return false;
		}
		
		if (param["type"] == "frame")
		{
			if (! f.openwrite ("index.html"))
			{
				error (CoreModule::E_OTHER, "Could not write indexfile");
				return false;
			}
			
			
			f.writeln ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 "
					   "Frameset//EN\" \"http://www.w3.org/TR/html4/frameset.dtd\">");
			f.writeln ("<HTML><HEAD><TITLE>%s</TITLE></HEAD>" %format (id));
			f.writeln ("<FRAMESET>");
			f.writeln ("  <FRAME src=\"%s\">" %format (param["target"]));
			f.writeln ("</FRAMESET>");
			f.writeln ("</HTML>");
			f.close ();
			

			string fddir = "/var/www/forwards/%s" %format (id);
			authd.makeDir (fddir);
			
			if (! authd.installFile ("index.html", fddir))
			{
				error (CoreModule::E_AUTHD, authd.error); return false;
			}
		}
			
		if (! authd.installFile (fname, CONFDIR))
		{
			error (CoreModule::E_AUTHD, authd.error);
			return false;
		}
		
		if (! authd.reloadService (SVCNAME))
		{
			error (CoreModule::E_AUTHD, authd.error);
			return false;
		}
		
		return true;
	}
				 
	// ----------------------------------------------------------------------
	// REMOVE
	// ----------------------------------------------------------------------
	bool remove (void)
	{
		string fnam = "%s.conf" %format (id);
		string fpat = CONFDIR "/%s" %format (fnam);
		
        authd.deleteFile (fpat);
		
		fs.rm (fnam);
		authd.reloadService (SVCNAME);
		return true;
	}
};

// ==========================================================================
// CLASS ApacheForwardModule -> ApacheForward.module
// ==========================================================================
class ApacheForwardModule : public CoreModule
{
public:
	ApacheForwardModule (void)
		: CoreModule ("ApacheForward.module")
	{
	}
	 
	~ApacheForwardModule (void)
	{
	}
					 
	HTTPForward httpforward;
};

IMPLEMENT (ApacheForwardModule);
