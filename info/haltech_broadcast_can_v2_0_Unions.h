/* This is an ALWAYS automatically generated file based on the database ... */
/*It is recommended to change the database directly. If you want to prevent overwritten header files, make a copy of it and ensure you include the intended version in your node simulation.*/

#pragma once
#pragma pack(pop,8)

#ifndef MESSAGENAME_Eng1
#define MESSAGENAME_Eng1
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int RPM              : 16 ;
	    unsigned int MAF_Pressure     : 16 ;
	    unsigned int Throttle_Pos     : 16 ;
	    unsigned int Coolant_Pressure : 16 ;

	} __attribute__((__packed__));
}Eng1_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Eng1_         m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Eng1;
#endif /* MESSAGENAME_Eng1 */


#ifndef MESSAGENAME_Temp1
#define MESSAGENAME_Temp1
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int EGT01            : 16 ;
	    unsigned int EGT02            : 16 ;
	    unsigned int EGT03            : 16 ;
	    unsigned int EGT04            : 16 ;

	} __attribute__((__packed__));
}Temp1_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Temp1_        m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Temp1;
#endif /* MESSAGENAME_Temp1 */


#ifndef MESSAGENAME_Temp2
#define MESSAGENAME_Temp2
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int EGT05            : 16 ;
	    unsigned int EGT06            : 16 ;
	    unsigned int EGT07            : 16 ;
	    unsigned int EGT08            : 16 ;

	} __attribute__((__packed__));
}Temp2_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Temp2_        m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Temp2;
#endif /* MESSAGENAME_Temp2 */


#ifndef MESSAGENAME_Temp3
#define MESSAGENAME_Temp3
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int EGT09            : 16 ;
	    unsigned int EGT10            : 16 ;
	    unsigned int EGT11            : 16 ;
	    unsigned int EGT12            : 16 ;

	} __attribute__((__packed__));
}Temp3_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Temp3_        m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Temp3;
#endif /* MESSAGENAME_Temp3 */


#ifndef MESSAGENAME_Temp4
#define MESSAGENAME_Temp4
typedef union 
{
	unsigned char      m_aucData[64];
	unsigned short int m_auwData[4];
	unsigned long  int m_aulData[2];

	struct
	{
	    unsigned int Coolant_Temp     : 16 ;
	    unsigned int Air_Temp         : 16 ;
	    unsigned int Fuel_Temp        : 16 ;
	    unsigned int Oil_Temp         : 16 ;

	} __attribute__((__packed__));
}Temp4_;
typedef struct 
{
	unsigned int  m_unMsgID       ;
	unsigned char m_ucEXTENDED    ;
	unsigned char m_ucRTR         ;
	unsigned char m_ucDLC         ;
	unsigned char m_ucChannel	;
	Temp4_        m_sWhichBit     ;

	UINT m_ulTimeStamp;
	bool		     m_bCANFD        ;

} Temp4;
#endif /* MESSAGENAME_Temp4 */
