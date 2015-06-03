#ifndef FORMATTEDRECORD_H_
#define FORMATTEDRECORD_H_

#include "Endianness.h"


/* ---------------------------------------------------------------------- */

  class FormattedRecord{
  public:
    struct Generic
    {
#if     ENDIANNESS_IS_BIG
      
      unsigned int datakey: 1;
      unsigned int headertwokey: 1;
      unsigned int headerkey: 1;
      unsigned int unused2: 29;
      
#elif   ENDIANNESS_IS_LITTLE
      
      unsigned int unused2: 29;
      unsigned int headerkey: 1;
      unsigned int headertwokey: 1;
      unsigned int datakey: 1;
      
#else
      
#error  ENDIANNESS is not defined
#endif
    };
    struct Header
    {
#if     ENDIANNESS_IS_BIG
      
      unsigned int unused1: 2;
      unsigned int key: 1;
      unsigned int link: 4;
      unsigned int l1id: 12;
      unsigned int bxid: 13;
      
#elif   ENDIANNESS_IS_LITTLE
      
      unsigned int bxid: 13;
      unsigned int l1id: 12;
      unsigned int link: 4;
      unsigned int key: 1;
      unsigned int unused1: 2;
      
#else
      
#error  ENDIANNESS is not defined
#endif
    };
    
    
    struct HeaderTwo
    {
#if     ENDIANNESS_IS_BIG

      unsigned int unused1: 1;
      unsigned int key: 1;
      unsigned int unused2: 22;
      unsigned int rce: 8;

#elif   ENDIANNESS_IS_LITTLE

      unsigned int rce: 8;
      unsigned int unused2: 22;
      unsigned int key: 1;
      unsigned int unused1: 1;

#else

#error  ENDIANNESS is not defined
#endif
    };


    struct Data
    {
#if     ENDIANNESS_IS_BIG
      
      unsigned int key: 1;
      unsigned int unused1: 3;
      unsigned int fe: 4;
      unsigned int tot: 8;
      unsigned int col: 7;
      unsigned int row: 9;
      
#elif   ENDIANNESS_IS_LITTLE
      
      unsigned int row: 9;
      unsigned int col: 7;
      unsigned int tot: 8;
      unsigned int fe: 4;
      unsigned int unused1: 3;
      unsigned int key: 1;
      
#else
      
#error  ENDIANNESS is not defined
#endif
    };

  
      union Record
      {
	unsigned int  ui;
	Generic       ge;
	Header        he;
	Data          da;
	HeaderTwo     ht;
      };

    enum type{HEADER=0x20000000, HEADERTWO=0x40000000, DATA=0x80000000};
    
    FormattedRecord(FormattedRecord::type ty){
      m_record.ui=ty;
    }
    FormattedRecord(unsigned& wd){
      m_record.ui=wd;
    }
    
    bool isHeader(){return m_record.ge.headerkey;}
    bool isHeaderTwo(){return m_record.ge.headertwokey;}
    bool isData(){return m_record.ge.datakey;}
    
    unsigned getLink(){return m_record.he.link;}
    void setLink(unsigned link){m_record.he.link=link;}
    unsigned getBxid(){return m_record.he.bxid;}
    void setBxid(unsigned bxid){m_record.he.bxid=bxid;}
    unsigned getL1id(){return m_record.he.l1id;}
    void setL1id(unsigned l1id){m_record.he.l1id=l1id;}
    
    unsigned getRCE(){return m_record.ht.rce;}
    void setRCE(unsigned rce){m_record.ht.rce=rce;}
    
    unsigned getFE(){return m_record.da.fe;}
    void setFE(unsigned fe){m_record.da.fe=fe;}
    unsigned getToT(){return m_record.da.tot;}
    void setToT(unsigned tot){m_record.da.tot=tot;}
    unsigned getCol(){return m_record.da.col;}
    void setCol(unsigned col){m_record.da.col=col;}
    unsigned getRow(){return m_record.da.row;}
    void setRow(unsigned row){m_record.da.row=row;}

    unsigned getWord(){return m_record.ui;}

  private:
    Record m_record;
  };

#endif
