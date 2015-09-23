/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: msg-e.h,v 1.2 2001/02/23 22:10:55 takayuki Exp $
 */

#ifndef __MESSAGE_H
#define __MESSAGE_H

#define MSG_INTERNAL			"Internal error"
#define MSG_OR					" or "

#define MSG_UNKNOWN				"Unknown error"

#define MSG_NULLAPIASSIGNMENT	"Assigning a Static API with NULL pointer"
#define MSG_NULLSERASSIGNMENT	"Assigning a Serializer with NULL pointer"
#define MSG_ILLEGALPRAMDESC		"Illegal parameter descriptor (Unterminated descriptor)"
#define MSG_SYNTAXERROR			"Parameter descriptor syntax error"
#define MSG_APINOTEXIST			"That static API does not exist"
#define MSG_FATALONAPI			"Fatal error occured during Static API processing"
#define MSG_VALIDATIONFAIL		"Fatal error occured during validation checking"
#define MSG_FATALONOUTPUT		"Fatal error occured during output"
#define MSG_VALIDATIONDEADLOCK	"Validation step comes to a deadlock"
#define MSG_COULDNOTOPEN		"Specified file could not open"
#define MSG_COULDNOTCREATE		"Specified file could not create"
#define MSG_ILLEGALTYPE			"Type of the parameter given is not match the specification"
#define MSG_TOOMANYPARAM		"Too many parameters"
#define MSG_TOOFEWPARAM 		"Too few parameters"
#define MSG_ALREADYEXISTED		"The object you specified has already existed"
#define MSG_ALREADYASSIGNED		"The ID is already assigned for the other object"
#define MSG_ILLEGALIDENTIFIER	"Illegal identifier specified"
#define MSG_NOTEXIST			"There is no object specified"
#define MSG_ILLEGALIDENTIFIER2	"\n  (discontinuance, negative value, out of range, and so on)"
#define MSG_NONNULLASSIGNED		"Non-NULL pointer specified as a buffer of the object"

#endif
