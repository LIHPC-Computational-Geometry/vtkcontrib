#ifndef VTK_CONTRIB_H
#define VTK_CONTRIB_H


#include <string>


/**
 * Classe permettant de connaître la version de cette bibliothèque.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class VtkContribInfos
{
	public :

	/**
	 * @return		La version de cette bibliothèque
	 */
	static const std::string& getVersion ( )
	{ return _version; }


	protected :

	/**
	 * Constructeurs, opérateur = et destructeur : interdits.
	 */
	VtkContribInfos ( );
	VtkContribInfos (const VtkContribInfos& exc);
	VtkContribInfos& operator = (const VtkContribInfos& exc);
	~VtkContribInfos ( );


	private :

	/**
	 * La version de cette bibliothèque.
	 */
	static const std::string		_version;
};	// class VtkContribInfos


#endif	// VTK_CONTRIB_H
