#ifndef NANDGEOMETRYFROMUSER_HPP
#define NANDGEOMETRYFROMUSER_HPP

#include "NandGeometry.hpp"
class NandGeometryFromUser : public NandGeometry
{
public:
	NandGeometryFromUser(FtdiNand *fn, unsigned char *idbytes, char* geometry);
	virtual string getDesc();
	virtual long getPagesCount();
	virtual int getSizeMB();
	virtual int getPageSize();
	virtual int getOobSize();
	virtual bool isLargePage();
	virtual int getAddrByteCount();
	static string getTextTemplate();
private:
	string m_nandDesc;
	int m_nandPageSz;
	int m_nandOobSz;
	int m_nandaddrcyc;
	bool m_nandIsLP;
	long m_pagesCount;
};

#endif
