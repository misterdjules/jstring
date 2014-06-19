#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include <debug.h>

#include "jstring.h"

CREATE_DEBUG_CTX(jstring);

/*
 * Joins all the elements of the strings array "tokens" into a single
 * string with each token separated by the string "separator".
 * Returns the pointer to the newly allocated string if successful,
 * NULL otherwise.
 * If "separator" is NULL, the tokens are joined without using any
 * separator.
 */
char* jstr_join(const char** tokens, const char* separator)
{
	size_t total_size         = 0;
	unsigned int nb_tokens    = 0;
	const char** tokens_iter  = tokens;
	char* joined_string       = NULL;
	char* joined_string_start = NULL;

	assert(tokens);
	if (!tokens)
		return NULL;

	while (*tokens_iter)
	{
		total_size += strlen(*tokens_iter);
		++tokens_iter;
		++nb_tokens;
	}

	total_size = total_size + ((nb_tokens - 1) * strlen(separator));
	total_size += 1; /* NULL termination byte */

	joined_string = malloc(total_size * sizeof(**tokens));
	if (!joined_string)
		return NULL;

	joined_string[0] = '\0';

	tokens_iter = tokens;
	joined_string_start = joined_string;
	while (*tokens_iter)
	{
		jstring_debug("Joining token: [%s]\n", *tokens_iter);
		strncat(joined_string, *tokens_iter, strlen(*tokens_iter));

		/* Do not append separator if it's the last token to be joined */
		if (separator && *(tokens_iter + 1))
		{
			jstring_debug("Appending separator: [%s]\n", separator);
			strncat(joined_string + strlen(*tokens_iter), separator, strlen(separator));
		}

		joined_string += strlen(*tokens_iter) + (separator ? strlen(separator) : 0);

		++tokens_iter;
	}

	joined_string_start[total_size - 1] = '\0';

	jstring_debug("Joined string: [%s]\n", joined_string_start);
	jstring_debug("Total size: %d\n", total_size);

	return joined_string_start;
}

/*
 * Takes a format string "format" and a variable list of variables,
 * returns a newly allocated string that points to the formatted string.
 *
 * Returns NULL if format is NULL.
 *
 * Implementation is largely inspired by man snprintf, see "make_message"
 * implementation in the "EXAMPLE" section of the man page.
 */
char* jstr_format(const char* format, ...)
{
	int nb_bytes_written = 0;
	int buffer_size      = 100;
	char* buffer         = NULL, *adjusted_buffer = NULL;
	int buffer_too_small  = 1;
	va_list ap;

	buffer = malloc(buffer_size * sizeof(*buffer));
	if (!buffer)
		return NULL;

	while (buffer_too_small)
	{
		va_start(ap, format);
		/*
		 * First, try to write the string into the currently allocated space
		 * that may be too small.
		 */
		nb_bytes_written = vsnprintf(buffer, buffer_size, format, ap);
		va_end(ap);

		/*
		 * If vsnprintf could write the whole string, then the
		 * pointer to the formatted string is returned.
		 */
		if (nb_bytes_written > -1 && nb_bytes_written < buffer_size)
		{
			buffer_too_small = 0;
			return buffer;
		}

		if (nb_bytes_written > -1)
		{
			/*
			 * Otherwise, if vsnprintf's return value is > -1,
			 * it means that we're using glibc 2.1 and it contains
			 * the number of bytes necessary to write the whole
			 * string. Realloc the buffer to have this size so
			 * vsnprintf will be successful during the next iteration.
			 */
			buffer_size = nb_bytes_written;
		}
		else
		{
			/*
			 * If vsnprintf's return value is -1 or less, it means
			 * that we're using glibc 2.0, and there's no way to get the
			 * exact size that would be needed to write the whole string, so
			 * we make a guess by doubling the buffer's size.
			 */
			buffer_size *= 2;
		}

		adjusted_buffer = realloc(buffer, buffer_size);
		if (!adjusted_buffer)
		{
			free(buffer);
			return NULL;
		}
		else
		{
			buffer = adjusted_buffer;
		}
	}

	return NULL;
}