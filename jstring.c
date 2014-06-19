#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <debug.h>

#include "jstring.h"

CREATE_DEBUG_CTX(jstring);

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

	tokens_iter = tokens;
	joined_string_start = joined_string;
	while (*tokens_iter)
	{
		strncat(joined_string, *tokens_iter, strlen(*tokens_iter));

		/* Do not append separator if it's the last token to be joined */
		if (separator && *(tokens_iter + 1))
			strncat(joined_string + strlen(*tokens_iter), separator, strlen(separator));

		joined_string += strlen(*tokens_iter) + (separator ? strlen(separator) : 0);

		++tokens_iter;
	}

	jstring_debug("joined string: [%s]\n", joined_string_start);
	jstring_debug("total size: %d\n", total_size);

	joined_string_start[total_size - 1] = '\0';

	return joined_string_start;
}