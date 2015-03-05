/** 
 * \defgroup RESULT Common Result Codes
 *
 * \{
 */

#ifndef RESULT_H
#define RESULT_H

enum result_code {
	/* The first three result codes are the important ones */
	RES_OK = 0,				/**< Everything is fine */
	RES_WOULD_BLOCK = 1,	/**< 
							 * Function returns early although operation is not finished
							 * because it would block otherwise. Function has to be called
							 * again later to complete the operation.
							 */
	RES_EOF = 2,			/**< 
							 * End of file reached. This will be returned by the file_read() function 
							 * if zero bytes were read because the end of file was reached.
							 */
	RES_ERROR = 3,			/**< A general error occurred */

	RES_TIMEOUT,			/**< Timeout occurred. */
	RES_NOT_SUPP,			/**< Requested operation or function is not supported */
	RES_NOT_FOUND,			/**< Something (like a file, etc) was not found. */
	RES_NO_PERM,			/**< No permission for requested operation */
	RES_IO_ERROR,			/**< Underlying IO Operation failed */
	RES_HW_ERROR,			/**< A hardware error was detected. */
	RES_OUT_OF_MEM,			/**< Out of memory. This will be returned by a function if a memory allocation fails. */
	RES_WARNING,			/**< This is not an error, just a warning */
	RES_BAD_RESPONSE		/**< A bad response from a communication partner was received. */
	RES_INV_ARG,			/**< An invalid argument was given to a function. */

};

#endif

/** \} */
