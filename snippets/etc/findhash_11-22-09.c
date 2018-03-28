/*
Released under the GNU General Public License
(http://www.gnu.org/licenses/gpl.html)

gcc -o findhash findhash.c -lssl32 -leay32

findhash -ripemd160 test

@echo off
for %a IN (C:\Windows\System32\*.*) DO findhash "%a" >> ..\hash1.out
for %a IN (C:\Windows\System32\*.*) DO findhash "%a" ..\hash1.out | grep -v ^PASS

Jay Satiro <raysatiro@users.sourceforge.net>
*/

/*
Bugfix 11-22-09: in previous revisions if file-to-hash was exactly the size of 
one block its data would be processed twice, resulting in an incorrect hash.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h> 
#include <assert.h>

#include <openssl/evp.h>

#ifdef __GNUC__
#define FORCE_INLINE __attribute__((always_inline))
#elif _MSC_VER
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __inline__
#endif

#ifndef HASH_ALGORITHM
#define HASH_ALGORITHM "sha1"
#endif
char *hash_algorithm_name = HASH_ALGORITHM;

unsigned int opt_verbose = 0;
unsigned int opt_hide_path = 0;

/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/secauthn/security/unicode_string.asp
BUFSIZE should be at least max hash entry size in bytes + actual desired buffer size in bytes
.
max hex hash size bytes (EVP_MAX_MD_SIZE * 2) + 
max separator length bytes (" *" ....at least 2 ) + 
max filename bytes (USHORT max (65535)) ==
max hash entry size in bytes
.
BUFSIZE no mod if divided by EVP_MAX_MD_SIZE * 2
EVP_MAX_MD_SIZE is 64; defined in evp.h in openssl 0.9.8l; longest known is SHA512.
.
calling minimum BUFSIZE at 65536 * 2 for now == 131072
0 == (BUFSIZE % EVP_MAX_MD_SIZE * 2). checked at runtime
in practice the buffer adjusts and most is used for reading in data
*/
#define BUFSIZE 131072
//#define BUFSIZE 262144
//#define BUFSIZE 524288
//#define BUFSIZE 1048576
char main_buffer[ BUFSIZE ] = { '\0', };

//functions:
int check_hashfile();
char *find_eol_in_main_buffer( char *start, char *end );
FORCE_INLINE char *find_eol_char( char *buffer, int count );
void print_usage_and_quit();
void quit_on_error( char *error_message );

char *progname = NULL;
EVP_MD_CTX mdctx; /* { '\0', '\0', '\0', '\0' } zeroed out by EVP_MD_CTX_init() */

unsigned hash_len = 0;
char hash_value[ EVP_MAX_MD_SIZE * 2 + 1 ] = { '\0', };
unsigned _binary_hash_len = 0;
unsigned char _binary_hash_value_unterminated[ EVP_MAX_MD_SIZE ] = { '\0', };
const char *separator = " *";

char *filename_file_to_hash = NULL;
FILE *fp_file_to_hash = NULL;

char *filename_hashfile_to_check_against = NULL;
FILE *fp_hashfile_to_check_against = NULL;

int main( int argc, char *argv[] )
{
	_fmode = _O_BINARY;
	
	progname = argv[ 0 ];
	
	// malloc returns an aligned memory address, so this should be no problem:
	assert( (uint64_t)main_buffer % sizeof( uint32_t ) == 0 );
	
	int block_size, i, ret, mod;
	block_size = i = ret = mod = 0;
	
	OpenSSL_add_all_digests();	
		
	for( i = 1; i < argc; ++i )
	{
		if( argv[ i ][ 0 ] == '-' )
		{
			if( strlen( argv[ i ] ) > 2 )
			{
				// specific hash algorithm specified on the command line
				hash_algorithm_name = &argv[ i ][ 1 ];
			} else {
				switch( argv[ i ][ 1 ] )
				{
					case 'v':
						// go verbose
						opt_verbose = 1;
						break;
					case 'b':
						opt_hide_path = 1;
						break;
					default:
						fprintf( stderr, "Error: unknown option '%s'\n\n", argv[ i ] );
						print_usage_and_quit();
						break;
				}
			}
		} else {
			if( !filename_file_to_hash )
				filename_file_to_hash = argv[ i ];
			else if( !filename_hashfile_to_check_against )
				filename_hashfile_to_check_against = argv[ i ];
			else
				quit_on_error( "Error: Command line parsing failed: Too many parameters." );
		}
	}
	
	if( !EVP_get_digestbyname( hash_algorithm_name ) )
	{
		fprintf( stderr, "Error: hash algorithm not found %s\n\n", hash_algorithm_name );
		print_usage_and_quit();
	}

	
	if( !filename_file_to_hash )
		print_usage_and_quit();

	if( ( fp_file_to_hash = fopen( filename_file_to_hash, "rb" ) ) == NULL )
	{
		fprintf( stderr, "Error: couldn't open %s\n\n", filename_file_to_hash );
		print_usage_and_quit();
	}
	
	if( filename_hashfile_to_check_against
		&& ( fp_hashfile_to_check_against = fopen( filename_hashfile_to_check_against, "rb" ) ) == NULL )
	{
		fprintf( stderr, "Error: couldn't open %s\n\n", filename_hashfile_to_check_against );
		print_usage_and_quit();
	}
	
	//quit_on_error( "test" );//
	//assert( 0 );//
	
	EVP_MD_CTX_init( &mdctx );
	
	if( !EVP_DigestInit_ex( &mdctx, EVP_get_digestbyname( hash_algorithm_name ), NULL ) )
		quit_on_error( "EVP_DigestInit_ex() failed." );
	
	if( ( block_size = EVP_MD_CTX_block_size( &mdctx ) ) == 0 )
		quit_on_error( "EVP_MD_block_size() failed." );
	
	/* EVP_MAX_MD_SIZE == 64 in 0.9.8l; longest known is SHA512.*/	
	if( block_size > EVP_MAX_MD_SIZE )
		quit_on_error( "block_size > EVP_MAX_MD_SIZE" );
	
	if( BUFSIZE % block_size )
		quit_on_error( "BUFSIZE % block_size" );
	
	if( BUFSIZE < block_size )
		quit_on_error( "BUFSIZE < block_size" );
	
	
	while( ret = fread( main_buffer, 1, BUFSIZE, fp_file_to_hash ) )
	{
		if( ferror( fp_file_to_hash ) || ret < 0 )
			quit_on_error( "ferror() reading file-to-hash." );
		
		int blocks;
		blocks = ret / block_size;
		
		if( blocks )
		{
			if( !EVP_DigestUpdate( &mdctx, main_buffer, blocks * block_size ) )
				quit_on_error( "EVP_DigestUpdate() failed." );
		}
		
		mod = ret - ( blocks * block_size );
		if( mod )
		{
			if( !EVP_DigestUpdate( &mdctx, main_buffer + ( blocks * block_size ), mod ) )
				quit_on_error( "EVP_DigestUpdate() failed." );
		}
	}
	
	if( fclose( fp_file_to_hash ) == 0 )
		fp_file_to_hash = NULL;
	
	if( opt_hide_path )
	{
		char *temp_ptr1 = strrchr( filename_file_to_hash, '\\' );
		char *temp_ptr2 = strrchr( filename_file_to_hash, '/' );
		
		if( temp_ptr1 || temp_ptr2 )
		{
			// path found, point to filename without path.
			// file_to_hash has already been read and will not need to be opened again, so this is ok.
			filename_file_to_hash = ( ( temp_ptr1 > temp_ptr2 ) ? temp_ptr1 : temp_ptr2 ) + 1;
		}
	}
	
	if( !EVP_DigestFinal_ex( &mdctx, _binary_hash_value_unterminated, &_binary_hash_len ) )
		quit_on_error( "EVP_DigestFinal_ex() failed." );

	if( !EVP_MD_CTX_cleanup( &mdctx ) )
		quit_on_error( "EVP_MD_CTX_cleanup() failed." );
	
	/*
	for(i = 0; i < _binary_hash_len; i++)
		printf("%02x", _binary_hash_value_unterminated[i]);
	printf("\n");
	*/
	
	hash_len = _binary_hash_len * 2;
	
	for( i = 0; i < hash_len; ++i )
	{
		sprintf( &hash_value[ i * 2 ], "%02x", _binary_hash_value_unterminated[ i ] );
	}
	hash_value[ hash_len ] = '\0';
	
	
	if( !fp_hashfile_to_check_against )
	{
		char *temp_buffer;
		if( ( temp_buffer = calloc( strlen( hash_value ) + strlen( separator ) + strlen( filename_file_to_hash ) + 1, 1 ) ) == NULL )
			quit_on_error( "calloc() failed." );
		
		sprintf( temp_buffer, "%s%s%s", hash_value, separator, filename_file_to_hash );
		fprintf( stdout, "%s\n", temp_buffer );
		return 0;
	}
	
	return check_hashfile();
}


int check_hashfile()
{
	assert( separator );
	assert( main_buffer );
	assert( hash_value );
	assert( hash_len );
	assert( filename_file_to_hash );
	assert( fp_hashfile_to_check_against );
	
	main_buffer[ 0 ] = '\0'; // cap main
	
	int separator_len = strlen( separator );
	int filename_file_to_hash_len = strlen( filename_file_to_hash );
	int read_line_buffer_len = hash_len + separator_len + filename_file_to_hash_len;
	
	assert( read_line_buffer_len + 1 < BUFSIZE );
	
	memset( main_buffer, ' ', read_line_buffer_len );
	/*
	the first bytes (read_line_buffer_len) of the main_buffer will possibly 
	eventually contain trailing data from the previous read
	here it's initialized as whitespace
	*/
	
	int ret = 0;
	int bytes_read = 0;
	int bytes_total = 0;
	int trailing_data_len = 0;
	char *main_buffer_ptr = NULL;
	char *ptr_to_end_of_line = NULL;
	char *ptr_to_end_of_buffer = NULL;
	
	while( ret = fread( main_buffer + read_line_buffer_len, 1, BUFSIZE - read_line_buffer_len - 1, fp_hashfile_to_check_against ) )
	{
		if( ferror( fp_hashfile_to_check_against ) || ret < 0 )
			quit_on_error( "ferror() reading hashfile." );
		
		bytes_read = 0;
		bytes_total = ret + trailing_data_len;
		ptr_to_end_of_buffer = main_buffer + read_line_buffer_len + ret;
		
		ptr_to_end_of_line = NULL;
		//^^this pointer should not be used outside of the loop below (it's not reset in all situations)
		
		*ptr_to_end_of_buffer = '\0';
		//cap main to stop strpbrk from reading past the part of the buffer we are working with
		
		main_buffer_ptr = main_buffer + ( read_line_buffer_len - trailing_data_len );
		// rem there is no trailing data on the first read
		
		while( bytes_read < bytes_total )
		{
			/*if( main_buffer_ptr == ptr_to_end_of_buffer )
			{
				fprintf( stderr, "main_buffer_ptr == ptr_to_end_of_buffer\n" );
				fprintf( stderr, "bytes read:\t%d\nbytes total:\t%d\n", bytes_read, bytes_total );
			}
			*/
			ptr_to_end_of_line = find_eol_in_main_buffer( main_buffer_ptr, ptr_to_end_of_buffer );
			
			/*{ char *tmp_ptr; tmp_ptr = strpbrk( main_buffer_ptr, "\x0d\x0a" );
			fprintf( stderr, "tmp: %d, eolptr: %d\n", tmp_ptr, ptr_to_end_of_line );
			assert( tmp_ptr == ptr_to_end_of_line );
			}*/
			if( ptr_to_end_of_line == NULL )
			{
				// no eol char found
				break;
			}
			
			bytes_read += ( ptr_to_end_of_line - main_buffer_ptr ) + 1;
			
			if( ptr_to_end_of_line == main_buffer_ptr )
			{
				//advance past eol chars to get to next line.
				//advance pointer by a single byte
				++main_buffer_ptr;
				//fprintf( stderr, "advancing...\n" );
				continue;
			}
			
			//fprintf( stderr, "diff: %d rlblen: %d fn: %.*s\n", ptr_to_end_of_line - main_buffer_ptr, read_line_buffer_len, filename_file_to_hash_len, ptr_to_end_of_line - filename_file_to_hash_len);
			
			if( ptr_to_end_of_line - main_buffer_ptr != read_line_buffer_len )
			{
				// line size differs.
				main_buffer_ptr = ptr_to_end_of_line + 1;
				continue;
			}
			
			/* other conditions having been tested for, at this point the length of the line 
			found in hashfile is the same as the length of line we are searching for
			*/
			
			//check for filename_file_to_hash match:
			if( !strncmp( ptr_to_end_of_line - filename_file_to_hash_len - separator_len, separator, separator_len ) 
				&& !strncasecmp( ptr_to_end_of_line - filename_file_to_hash_len, filename_file_to_hash, filename_file_to_hash_len ) )
			{
				// filename_file_to_hash found, check for hash_value match:
				if( !strncasecmp( ptr_to_end_of_line - read_line_buffer_len, hash_value, hash_len ) )
				{
					// hash_value & filename_file_to_hash match:
					fprintf( stdout, "PASS: %s\n", filename_file_to_hash );
					if( opt_verbose )
					{
						fprintf( stdout, "\nThe computed file's hash matches the hash in the hashfile.\n" );
						fprintf( stdout, "Computed:\n%s%s%s\n", hash_value, separator, filename_file_to_hash );
						fprintf( stdout, "Hash found in hashfile:\n%.*s\n", read_line_buffer_len, ptr_to_end_of_line - read_line_buffer_len );
					}
					return 0;
				}
				else // filename_file_to_hash matches, but hash_value does NOT match
				{
					fprintf( stdout, "FAIL: %s\n", filename_file_to_hash );
					if( opt_verbose )
					{
						fprintf( stdout, "\nThe computed file's hash does NOT match the hash in the hashfile.\n" );
						fprintf( stdout, "Computed:\n%s%s%s\n", hash_value, separator, filename_file_to_hash );
						fprintf( stdout, "Hash found in hashfile:\n%.*s\n", read_line_buffer_len, ptr_to_end_of_line - read_line_buffer_len );
					}
					return 1;
				}
				assert( 0 ); // it shouldn't come to this point (all conditions should be tested for)
			} else {
				// same size line was found, but it does not contain filename_file_to_hash match
				main_buffer_ptr = ptr_to_end_of_line + 1;
				if( bytes_read < bytes_total && *main_buffer_ptr == '\x0a' ) 
				{
					//speedup for windows bec \x0d\x0a
					++bytes_read;
					++main_buffer_ptr;
				}
				continue;
			}
			assert( 00 ); // it shouldn't come to this point (all conditions should be tested for)
		}
		assert( bytes_read <= bytes_total );
		
		if( bytes_read < bytes_total )
		{
			// move trailing data to the beginning of the buffer:
			int temp1; temp1 = bytes_total - bytes_read;
			trailing_data_len = ( temp1 > read_line_buffer_len ) ? read_line_buffer_len : temp1;
			assert( trailing_data_len <= read_line_buffer_len );
			strncpy( main_buffer + ( read_line_buffer_len - trailing_data_len ), main_buffer + ( read_line_buffer_len + ret - temp1 ), trailing_data_len );
			assert( NULL == memchr( main_buffer + ( read_line_buffer_len - trailing_data_len ), '\0', trailing_data_len ) );
			//mingw missing strnlen
			//fprintf( stderr, "trailing data: %d %.*s %d\n", trailing_data_len, trailing_data_len, main_buffer + ( read_line_buffer_len - trailing_data_len ), read_line_buffer_len);
			//fprintf( stderr, "^^filename: %s\n", filename_file_to_hash );
		} else {
			trailing_data_len = 0;
		}
	}
	
	fprintf( stdout, "MISSING: %s\n", filename_file_to_hash );
	if( opt_verbose )
	{
		fprintf( stdout, "\nThe filename was not found in the hashfile.\n" );
		fprintf( stdout, "Computed:\n%s%s%s\n", hash_value, separator, filename_file_to_hash );
	}
	return 2;
}

//http://graphics.stanford.edu/~seander/bithacks.html#HasLessInWord
#define hasless(x,n) (((x)-~0UL/255*(n))&~(x)&~0UL/255*128)
#define hasbetween(x,m,n) \
((~0UL/255*(127+(n))-((x)&~0UL/255*127)&~(x)&((x)&~0UL/255*127)+~0UL/255*(127-m))&~0UL/255*128)
//When m < n, this technique tests if a word x contains an unsigned byte value, such that m < value < n.

char *find_eol_in_main_buffer( char *start, char *end )
{
	// i wrote this function because of null eol characters, otherwise i would use strpbrk()
	
	char *temp_ptr1 = NULL;
	
	//start is not necessarily aligned because it is probably not the address returned by malloc
	//assert( start >= main_buffer );//
	
	int number_of_bytes_not_aligned = start - main_buffer;
	if( number_of_bytes_not_aligned > sizeof( uint32_t ) )
		number_of_bytes_not_aligned %= sizeof( uint32_t );
	if( number_of_bytes_not_aligned )
	{
		number_of_bytes_not_aligned = sizeof( uint32_t ) - number_of_bytes_not_aligned;
		
		if( number_of_bytes_not_aligned > end - start )
			number_of_bytes_not_aligned = end - start;
		
		if( number_of_bytes_not_aligned 
			&& ( temp_ptr1 = find_eol_char( start, number_of_bytes_not_aligned ) ) )
			return temp_ptr1;
		
		start += number_of_bytes_not_aligned;
	}
	
	
	int number_of_words = ( end - start ) / sizeof( uint32_t );
	
	while( number_of_words )
	{
		if( hasless( *(uint32_t *)start, 14 ) ) // if the word has a byte in it <14
		{
			if( ( temp_ptr1 = find_eol_char( start, sizeof( uint32_t ) ) ) )
				return temp_ptr1;
		}
		start += sizeof( uint32_t );
		--number_of_words;
	}
	
	return find_eol_char( start, end - start );
}

FORCE_INLINE char *find_eol_char( char *buffer, int count )
{
	while( count )
	{
		// looking for LF(10) or CR(13) or null(0) or EOT(3) or FF(12)
		// don't know what uses eot or ff as eol... put here for posterity
		if( *buffer == '\x0a' || *buffer == '\x0d' || *buffer == '\x00' || *buffer == '\x03' || *buffer == '\x0c' )
		{
			/* not needed for now. 
			null can still be an eol char but no other string functions are used that read the eol 11/12/09
			if( *buffer == '\0' )
				// null byte found. in certain scenarios null can be an eol char.
				// replace null char with linefeed (otherwise other string functions could cough)
				*buffer = '\x0a';
			*/
			return buffer;
		}
		++buffer;
		--count;
	}
	
	return NULL;
}

void print_usage_and_quit()
{
	assert( progname );
	
	fprintf( stderr, "Usage:\t%s [-<hash-algorithm>] [-b] <file-to-hash>\n", progname );
	fprintf( stderr, "Calculate the %s hash and print it\n\n", hash_algorithm_name );
	
	fprintf( stderr, "Usage:\t%s [-<hash-algorithm>] [-b] <file-to-hash> [hashfile-to-check]\n", progname );
	fprintf( stderr, "Calculate the %s hash and check for its existence in a hashlist.\n\n", hash_algorithm_name );
	
	fprintf( stderr, "Options:\n"
		"\t-v   verbose\n"
		"\t-b   hide filepath when printing [or checking against hashlist]\n"
		"\t-sha256, -sha512, etc. to change the default hash algorithm\n\n"
	);
	
	fprintf( stderr, "Returns errorlevel:\n"
		"\t0 (PASS) if successful [and hash exists in hashlist]\n"
		"\t1 (FAIL) if file's computed hash is different from its hash in hashfile\n"
		"\t2 (MISSING) if file's name is not found as an entry in hashfile\n"
		"\t>= 3 if there's an internal error (openssl, memory, file, assert)\n"
	);
	fprintf( stderr, "\nWritten by Jay Satiro on behalf of the GetGnuWin32 project.\n" );
	fprintf( stderr, "<raysatiro@users.sourceforge.net>\n" );
	exit( 3 );
}


void quit_on_error( char *error_message )
{
	fprintf( stderr, "Error: %s\n\n", error_message);	
	if( fp_file_to_hash ) fclose( fp_file_to_hash );
	if( fp_hashfile_to_check_against ) fclose( fp_hashfile_to_check_against );
	EVP_MD_CTX_cleanup( &mdctx );
	print_usage_and_quit();
}
