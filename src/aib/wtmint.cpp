// Copyright (c) 2014-2015 The AIB developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wtmint.h"
//#include "main.h"

/*
int64 GetBlockValue(){
		return 0;	
	}
*/
namespace WTMint
{
 

	int64 GetBlockValue(int diffTime,int nBits,int nHeight, int64 nFees, uint256 prevHash)
	{
			
				//Temple - diffblock coin implmented base on difficulty 
				if (nHeight==0)
					return 0;
				
				if (nHeight==1)
					return  41092653589793238; //5,000,000 AIB for about 10 years
					
				if (nHeight>1 && nHeight<=9)
					return 387500000000000000; 
					

				 return 1 * COIN + nFees;			
				
	}
}		 
				 