#include "Utils.hpp"



void Log(int severity, const char *fmt, ...)
{
    /* Determine strings for the type and colour */
    const char *type;
    const char *color;
    switch (severity)
    {
    case LOG_INFO:
        type = "info";
        color = CONSOLE_COLOR_GREEN;
        break;
    case LOG_ERROR:
        type = "error";
        color = CONSOLE_COLOR_RED;
        break;
    case LOG_WARNING:
        type = "warning";
        color = CONSOLE_COLOR_PURPLE;
        break;
    default:
        break; /* Unreachable */
    }

    /* Obtain the current date and time */
    time_t rawTime;
    struct tm *timeInfo;
    char timeBuffer[80];

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(timeBuffer, sizeof(timeBuffer), "[%H:%M:%S]", timeInfo);

    /* Format for printing to the console (with colours) */
    char consoleFormat[1024];
    snprintf(consoleFormat, 1024, "%s%s %s%s%s: %s\n", CONSOLE_COLOR_CYAN,
             timeBuffer, color, type, CONSOLE_COLOR_RESET, fmt);

    va_list argptr;

    /* Print to the console */
    va_start(argptr, fmt);
    vprintf(consoleFormat, argptr);
    va_end(argptr);
}



static const char b64enc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789" "+/";

char* b64_encode(const char *source, unsigned int length)
{
	unsigned int i, mlen, r_pos;
	unsigned short dif, j;
	unsigned int frame = 0;
	char out[5];
	char *res;

	mlen = 4 * length/3 + 1; /* +1 : returns a null-terminated string */
	if (length%3) {
		mlen += 4;
	}

	res = (char*) malloc(mlen);
	if (!res) {

		return NULL;
	}
	res[mlen-1] = '\0';
	out[4] = '\0';

	for (i=0; i<length; i+=3) {
		/*frame = 0; clean frame not needed because '>>' inserts '0' */
		dif = (length-i)/3 ? 3 : (length-i)%3; /* number of byte to read */
		for (j=0; j<dif; j++) {
			memcpy(((char*)&frame)+2-j, source+i+j, 1); /* copy 3 bytes in reverse order */
		}
		/*
		now 3 cases :
		. 3B red => 4chars
		. 2B red => 3chars + "="
		. 1B red => 2chars + "=="
		*/
		for (j=0; j<dif+1; j++) {
			out[j] = (char)((frame & 0xFC0000u) >> 18u); /* first 6 bits */
			out[j] = b64enc[(int)out[j]];
			frame = frame << 6u; /* next 6b word */
		}
		if (dif == 1) {
			out[2] = out [3] = '=';
		} else if (dif == 2) {
			out [3] = '=';
		}
		r_pos = (i/3)*4;
		strcpy(res+r_pos, out);
	}
	return res;
}

static char b64_value(char c) {
	if (c>='A' && c<='Z') {
		return c - 'A';
	} else if (c>='a' && c<='z') {
		return c - 'a' + 26;
	} else if (c>='0' && c<='9') {
		return c - '0' + 52;
	} else if (c=='+') {
		return 62;
	} else if (c=='/') {
		return 63;
	} else if (c=='=') {
		return 0;
	}
	return -1;
}



bool find_word(const std::string& haystack,const std::string& needle)
{
    auto index = haystack.find(needle);
    if (index == std::string::npos) return false;

    auto not_part_of_word = [&](int index){
        if (index < 0 || index >= haystack.size()) return true;
        if (std::isspace(haystack[index]) || std::ispunct(haystack[index])) return true;
        return false;
    };
    return not_part_of_word(index-1) && not_part_of_word(index+needle.size());
}

char* str_trim(char *str)
{
	int end = (int)(strlen(str)-1);
	while (end>=0 && isspace((unsigned char) str[end])) end--;
	str[end+1] = '\0';

	while(isspace((unsigned char) str[0])) str++;
	return str;
}

char* b64_decode(const char *source, unsigned int *rlength)
{ /* NULL terminated string */
	char *res, v;
	short j;
	unsigned int i;
	unsigned int in = 0;
	unsigned int src_len = (unsigned int)(strlen(source));

	if (!source)
        {
		Log(1, "Base64: invalid argument: source is NULL");
		return NULL;
	}

	if (src_len%4)
    {
		Log(1, "Base64: invalid source");
		return NULL; /* invalid source */
	}

	*rlength = (src_len/4)*3;
	res = (char*) malloc(*rlength);
	if (!res)
        {

		return NULL;
	}

	for (i=0; i<src_len; i+=4) {
		in = 0;

		for (j=0; j<4; j++) {
			v = b64_value(source[i+j]);
			if (v == -1) {
				Log(1, "Base64: invalid char '%c' in source", source[i+j]);
				goto cleanup;
			}
			in = in << 6u;
			in += v; /* add 6b */
		}
		for (j=0; j<3; j++) {
			memcpy(res+(i/4)*3+j, ((char*)&in)+2-j, 1); /* copy 3 bytes in reverse order */
		}
	}

	if (source[src_len-1] == '=') {
		(*rlength)--;
	}
	if (source[src_len-2] == '=') {
		(*rlength)--;
	}

	return res;

cleanup:
	free(res);
	return NULL;
}



unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int _base64Decode(const unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len )
{
    static char inalphabet[256], decoder[256];
    int c = 0, char_count, errors = 0;
    unsigned int input_idx = 0;
    unsigned int output_idx = 0;

    auto alphabetSize = sizeof(alphabet);
    for (size_t i = 0; i < alphabetSize; i++){
        inalphabet[alphabet[i]] = 1;
        decoder[alphabet[i]] = i;
    }

    char_count = 0;
    int bits = 0;
    for( input_idx=0; input_idx < input_len ; input_idx++ ) {
        c = input[ input_idx ];
        if (c == '=')
            break;
        if (c > 255 || ! inalphabet[c])
            continue;
        bits += decoder[c];
        char_count++;
        if (char_count == 4) {
            output[ output_idx++ ] = (bits >> 16);
            output[ output_idx++ ] = ((bits >> 8) & 0xff);
            output[ output_idx++ ] = ( bits & 0xff);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 6;
        }
    }

    if( c == '=' ) {
        switch (char_count) {
            case 1:
              Log(1, "base64Decode: encoding incomplete: at least 2 bits missing");
                errors++;
                break;
            case 2:
                output[ output_idx++ ] = ( bits >> 10 );
                break;
            case 3:
                output[ output_idx++ ] = ( bits >> 16 );
                output[ output_idx++ ] = (( bits >> 8 ) & 0xff);
                break;
            }
    } else if ( input_idx < input_len ) {
        if (char_count) {

           Log(1, "base64 encoding incomplete: at least %d bits truncated",
                    ((4 - char_count) * 6));

            errors++;
        }
    }

    *output_len = output_idx;
    return errors;
}

void _base64Encode( const unsigned char *input, unsigned int input_len, char *output )
{
    unsigned int char_count;
    unsigned int bits;
    unsigned int input_idx = 0;
    unsigned int output_idx = 0;

    char_count = 0;
    bits = 0;
    for( input_idx=0; input_idx < input_len ; input_idx++ ) {
        bits |= input[ input_idx ];

        char_count++;
        if (char_count == 3) {
            output[ output_idx++ ] = alphabet[(bits >> 18) & 0x3f];
            output[ output_idx++ ] = alphabet[(bits >> 12) & 0x3f];
            output[ output_idx++ ] = alphabet[(bits >> 6) & 0x3f];
            output[ output_idx++ ] = alphabet[bits & 0x3f];
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 8;
        }
    }

    if (char_count) {
        if (char_count == 1) {
            bits <<= 8;
        }

        output[ output_idx++ ] = alphabet[(bits >> 18) & 0x3f];
        output[ output_idx++ ] = alphabet[(bits >> 12) & 0x3f];
        if (char_count > 1) {
            output[ output_idx++ ] = alphabet[(bits >> 6) & 0x3f];
        } else {
            output[ output_idx++ ] = '=';
        }
        output[ output_idx++ ] = '=';
    }

    output[ output_idx++ ] = 0;
}

int base64Decode(const unsigned char *in, unsigned int inLength, unsigned char **out)
{
    unsigned int outLength = 0;

    //should be enough to store 6-bit buffers in 8-bit buffers
    *out = (unsigned char*)malloc(inLength / 4 * 3 + 1);
    if( *out ) {
        int ret = _base64Decode(in, inLength, *out, &outLength);

        if (ret > 0 )
        {

            Log(1,"Base64Utils: error decoding");

            free(*out);
            *out = nullptr;
            outLength = 0;
        }
    }
    return outLength;
}

int base64Encode(const unsigned char *in, unsigned int inLength, char **out)
{
    unsigned int outLength = (inLength + 2) / 3 * 4;

    //should be enough to store 8-bit buffers in 6-bit buffers
    *out = (char*)malloc(outLength+1);
    if( *out )
    {
        _base64Encode(in, inLength, *out);
    }
    return outLength;
}