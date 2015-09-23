/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: mpstrstream.h,v 1.2 2002/04/05 08:48:31 takayuki Exp $
 */


// $Header: /home/CVS/configurator/mpstrstream.h,v 1.2 2002/04/05 08:48:31 takayuki Exp $

#ifndef __MPFSTREAM
#define __MPFSTREAM

#ifdef _MSC_VER
  #pragma warning(disable:4786)
#endif

#include "except.h"
#include "garbage.h"

#include <fstream>
#include <strstream>
#include <list>
#include <string>

class mpstrstream : public Garbage
{
public:
	enum { DEFAULT = 0, NO_SERIALIZE = 1 };

protected:
	struct tagStreamPartInfo
	{
		std::string	    name;
		std::strstream  stream;
		int				flags;
	};

	std::string filename;

	std::list<struct tagStreamPartInfo *>           container;
	std::list<struct tagStreamPartInfo *>::iterator current;

	std::list<struct tagStreamPartInfo *>::iterator _find(const char * partname) const;

public:
	enum tagPosition { BEGIN, END };

	mpstrstream(void);
	mpstrstream(const std::string &);
	mpstrstream(const char * filename);
	~mpstrstream(void);

	bool             operator !  (void) const;
	std::strstream & operator *  (void) const;
	std::strstream & operator [] (const char *);
	std::strstream & operator [] (unsigned int);
	std::strstream & operator [] (enum tagPosition);
	std::strstream & operator ++ (void);
	std::strstream & operator -- (void);

	bool             is_valid(void)      const;
    const std::string &    getStreamName(void) const;
	const std::string &	 getPartName(void)   const;
	std::strstream & getStream(void)     const;

	std::strstream & moveto(const std::string & partname);
	std::strstream & moveto(const char * partname);
	std::strstream & moveto(unsigned int offset);
	std::strstream & moveto(enum tagPosition pos);
	std::strstream & movenext(void);
	std::strstream & moveprev(void);

	std::strstream & createpart(const char * partname, const char * second = 0, ... );
	std::strstream & insertpart(const char * partname, const char * successor = 0);
	void             erasepart(const char * partname);
	void             movepart(const char * partname, const char * successor = 0);

	void             assignFilename(const std::string &);
	void             assignFilename(const char *);
	void             serialize(void);
	std::ostream &   serialize(std::ostream &);

	int				 setf(const char * partname, int);
	int				 setf(int);

	template<class T>
	std::strstream & operator << (const T & src) { (*current)->stream << src; return (*current)->stream; };

};

inline mpstrstream::mpstrstream(void)
{	current = container.end();	}

inline mpstrstream::mpstrstream(const std::string & src)
{	current = container.end();	filename.assign(src);	}

inline mpstrstream::mpstrstream(const char * file)
{	current = container.end();	filename.assign(file);	}

inline const std::string & mpstrstream::getStreamName(void) const
{	return filename;	}

inline const std::string & mpstrstream::getPartName(void) const
{	return (*current)->name;	}

inline std::strstream & mpstrstream::getStream (void) const
{	return (*current)->stream;	}

inline std::strstream & mpstrstream::operator * (void) const
{	return getStream();	}

inline bool mpstrstream::is_valid(void) const
{	return !(container.end() == current);	}

inline bool mpstrstream::operator ! (void) const
{	return is_valid();	}

inline std::strstream & mpstrstream::operator [](const char * partname)
{	return moveto(partname);	}

inline std::strstream & mpstrstream::operator [](unsigned int offset)
{	return moveto(offset);	}

inline std::strstream & mpstrstream::operator [](enum tagPosition pos)
{	return moveto(pos);	}

inline std::strstream & mpstrstream::operator ++ (void)
{	return movenext();	}

inline std::strstream & mpstrstream::operator -- (void)
{	return moveprev();	}

inline std::strstream & mpstrstream::moveto(const std::string & partname)
{	return moveto(partname.c_str());	}

inline std::strstream & mpstrstream::movenext(void)
{	current++; return (*current)->stream;	}

inline std::strstream & mpstrstream::moveprev(void)
{	current--; return (*current)->stream;	}

inline void mpstrstream::assignFilename(const std::string & src)
{	filename.assign(src);	}

inline void mpstrstream::assignFilename(const char * src)
{	filename.assign(src);	}

inline int mpstrstream::setf(const char * partname, int flag)
{
	this->movepart(partname);
	return setf(flag);
}
#endif