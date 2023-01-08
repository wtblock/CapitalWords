/////////////////////////////////////////////////////////////////////////////
// Copyright © by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "KeyedCollection.h"

// an array of arguments passed into the program
vector<CString> m_arrArgs;

// a collection of unique names to be output
CKeyedCollection<CString, int> m_TotalOutput;

// collection of suffixes like Jr., Sr., III, etc.
CKeyedCollection<CString, int> m_suffixes;

// collection of titles like Mr., Mrs., Miss, Uncle, etc.
CKeyedCollection<CString, int> m_titles;

// words to ignore read from "words.txt" located with the
// executable
CKeyedCollection<CString, int> m_Ignore;

// create a concordance table output for auto-marking a Word document's index
bool m_bConcordance;

// if the input file is called "names.txt", then it is assumed to be the 
// output of this program when m_bConcordance is false, 
// i.e. First [Middle] Last is the input instead of individual words
// that is normally the input
bool m_bNames;

// handle some Unicode cases
const CString m_csAccent = _T( "â€\x99" );
const CString m_csOpenQuote = _T( "â€\x9c" );
const CString m_csCloseQuote = _T( "â€\x9d" );
const CString m_csDumbQuote = _T( "\"" );
const CString m_csDumbAccent( _T( "'" ));

// sentence terminators
const CString m_csNonPeriod( _T( ";:!?" ) );
const CString m_csTerminators( m_csNonPeriod + _T( "." ));

// word decorations
const CString m_csDecor = 
	m_csAccent + m_csOpenQuote + m_csCloseQuote +
	m_csDumbQuote + m_csDumbAccent + m_csNonPeriod;
const CString m_csDecorAll = m_csDecor + _T( "." );







