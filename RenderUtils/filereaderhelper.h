#pragma once

#include <string>
#include <iostream>
#include <fstream>

char* getStringFromFile( const char* path ) {

	int array_size = 5012;
	char * array = new char[ array_size ];
	int position = 0;

	std::fstream fin( path );

	if( fin.is_open() ) {
#ifdef _DEBUG
		std::string successfulMessage = "Opened the file at ";
		std::cout << std::endl << std::endl << successfulMessage.append( path ) << std::endl;
#endif
		while( !fin.eof() && position < array_size ) {
			fin.get( array[ position ] );
			position++;
		}
		array[ position - 1 ] = '\0';

#ifdef _DEBUG
		for( int i = 0; array[ i ] != '\0'; i++ ) 
			std::cout << array[ i ]; 
#endif
	} else {
		std::cout << "File was not opened";
		return nullptr;
	}

	return array;
}