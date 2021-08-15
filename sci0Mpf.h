#ifndef _SCI0MPF_H_
#define  _SCI0MPF_H_

#define MSG_DATA_SIZE 60

typedef struct t_sci0mpf { 
	T_MSG *header;
	char data[MSG_DATA_SIZE];
} T_SCI0MPF;
	
#endif /*  _SCI0MPF_H_ */
