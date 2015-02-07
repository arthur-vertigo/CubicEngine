/***

String Functions: Commonly used string functions

***/

#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H

#include <string>

class StringFunctions
{
public:
	// Will return the nth string value of a delimited string
	static std::string getPram( std::string& data, int position, char delimiter )
	{
		if( position < 1 )
		{
			position = 1;
		}

		int pram = 1;
		int start = -1;
		int end = data.size()-1;
		int size = 0;

		while( pram < position )
		{
			start = data.find( delimiter, start+1 );

			if( start == std::string::npos )
				return "";

			pram++;
		}

		start++;

		if( data[start] == delimiter )
			return "";

		end = data.find( delimiter, start+1 );

		if( end == std::string::npos )
		{
			size = data.size() - start;
		}
		else
		{
			size = end - start;
		}

		return data.substr( start, size ).c_str() ;
	}

	// Will return the number of prams in a delimited string
	static int getNumOfPrams( std::string& data, char delimiter )
	{
		int pram = 1;
		int start = -1;

		while( true )
		{
			start = data.find( delimiter, start+1 );

			if( start == std::string::npos )
				return pram;

			pram++;
		}
	}

	// Will remove all occurrences of specified string
	static void spliceOut( std::string& data, std::string unwanted, std::string replace = "" )
	{
		int occPos = data.find( unwanted );

		while( occPos != std::string::npos )
		{
			data.replace( occPos, unwanted.size(), replace );
			occPos = data.find( unwanted );
		}
	}

	// Retuns true if the string starts with the find value
	// -Returns true for comparison with "" 
	static bool beginsWith( std::string data, std::string find, bool caseSensitive = false )
	{
		int pos = 0;
		int length = find.size();

		if( !caseSensitive )
		{
			toLower( data );
			toLower( find );
		}
		
		while( pos < length )
		{
			if( data[pos] != find[pos] )
				return false;

			pos++;
		}

		return true;
	}

	// Will convert all uppercase letters to lower case
	static void toLower( std::string& data )
	{
		int pos = 0;
		int length = data.size();

		while( pos < length )
		{

			if( data[pos] > 64 && data[pos] < 91 )
				data[pos] += 32;

			pos++;
		}

	}

	// Will convert all lowercase letters to upper case
	static void ToUpper( std::string& data )
	{
		int pos = 0;
		int length = data.size();

		while( pos < length )
		{

			if( data[pos] > 96 && data[pos] < 123 )
				data[pos] -= 32;

			pos++;
		}

	}

	// Will remove the front n members of a string 
	static void removeFront( std::string& data, int num )
	{
		data.erase( 0, num );
	}

	// Will erase from either the front or the back until the find char is found
	static void EraseUntilCharFound( std::string& data, bool fromFront, char find, bool inclusive = false )
	{
		if( data.size() <= 0 )
			return;

		int pos = 0;

		if( !fromFront )
			pos = data.size()-1;

		while( data.size() != 0 )
		{
			if( data[pos] == find )
			{
				if( !inclusive )
					return;

				data.erase(pos,1);
				return;
			}

			data.erase(pos,1);

			if( !fromFront )
				pos--;
		}
	}
};  

#endif // !STRING_HFUNCTIONS_H
