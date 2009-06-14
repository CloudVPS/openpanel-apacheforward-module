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
	bool create (const value &env)
	{
		return update (env);
	}
	
	// ----------------------------------------------------------------------
	// UPDATE
	// ----------------------------------------------------------------------
	bool update (const value &env)
	{
		file f;
		string fname = "%s.conf" %format (id);
		
		if (! f.openwrite (fname))
		{
			seterror (CoreModule::E_OTHER, "Could not write file");
			return false;
		}
		
		try
		{
			f.writeln ("<VirtualHost *:80>");
			f.writeln ("  ServerAdmin webmaster@%s" %format (id));
			f.writeln ("  DocumentRoot /var/www/forwards/%s" %format (id));
			f.writeln ("  ServerName %s" %format (id));
			
			value aliases = listaliases (env);
			foreach (alias, aliases)
			{
				f.writeln ("  ServerAlias %s" %format (alias));
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
	bool remove (const value &env)
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
