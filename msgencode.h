#ifndef MSGENCODE_H
#define MSGENCODE_H

#define M_CHOKE 		(unsigned char) 0u
#define M_UNCHOKE 		(unsigned char) 1u
#define M_INTERESTED 		(unsigned char) 2u
#define M_NOT_INTERESTED	(unsigned char) 3u
#define M_HAVE 			(unsigned char) 4u
#define M_BITFIELD		(unsigned char) 5u
#define M_REQUEST 		(unsigned char) 6u
#define M_PIECE 		(unsigned char) 7u
#define M_CANCEL 		(unsigned char) 8u
#define M_PORT 			(unsigned char) 9u

#define H_INT_LEN		4	/* int_siz */
#define H_LEN			4	/* int_siz */
#define H_BASE_LEN		1	/* chr_siz */
#define H_HAVE_LEN		5	/* chr_siz + int_siz */
#define H_PIECE_LEN		9	/* chr_siz + int_siz*2 */
#define H_REQUEST_LEN		13	/* chr_siz + int_siz*3 */
#define H_CANCEL_LEN		13	/* chr_siz + int_siz*3 */
#define H_PORT_LEN		3	/* chr_siz + port_siz */

#endif
