#ifndef STRING_H_
#define STRING_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _WIN32
	#ifdef JSTRING_API_EXPORT
		#define JSTRING_API __declspec(dllexport)
	#elif !defined(JSTRING_API_STATIC)
		#define JSTRING_API __declspec(dllimport)
	#else
		#define JSTRING_API
	#endif /* JSTRING_API_EXPORT */
#else
	#define JSTRING_API
#endif /* _WIN32 */

JSTRING_API char* jstr_join(const char** tokens, const char* separator);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* STRING_H_ */