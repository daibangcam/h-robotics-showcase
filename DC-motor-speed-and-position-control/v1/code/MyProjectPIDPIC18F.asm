
_ngat:

;MyProjectPIDPIC18F.c,54 :: 		void ngat()iv 0x08
;MyProjectPIDPIC18F.c,56 :: 		if(TMR0IF_bit==1) {
	BTFSS       TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
	GOTO        L_ngat0
;MyProjectPIDPIC18F.c,57 :: 		TMR0IF_bit=0; TMR0L=0xB0;
	BCF         TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
	MOVLW       176
	MOVWF       TMR0L+0 
;MyProjectPIDPIC18F.c,58 :: 		TMR0H=0x3C;
	MOVLW       60
	MOVWF       TMR0H+0 
;MyProjectPIDPIC18F.c,59 :: 		count++;
	INFSNZ      _count+0, 1 
	INCF        _count+1, 1 
;MyProjectPIDPIC18F.c,60 :: 		coun++; count1++;
	INFSNZ      _coun+0, 1 
	INCF        _coun+1, 1 
	INFSNZ      _count1+0, 1 
	INCF        _count1+1, 1 
;MyProjectPIDPIC18F.c,61 :: 		output=xung*0.274; //xung*25*60/400=xung*3.75 (rpm)
	MOVF        _xung+0, 0 
	MOVWF       R0 
	MOVF        _xung+1, 0 
	MOVWF       R1 
	CALL        _int2double+0, 0
	MOVLW       186
	MOVWF       R4 
	MOVLW       73
	MOVWF       R5 
	MOVLW       12
	MOVWF       R6 
	MOVLW       125
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	CALL        _double2int+0, 0
	MOVF        R0, 0 
	MOVWF       _output+0 
	MOVF        R1, 0 
	MOVWF       _output+1 
;MyProjectPIDPIC18F.c,62 :: 		xung=0;
	CLRF        _xung+0 
	CLRF        _xung+1 
;MyProjectPIDPIC18F.c,66 :: 		saiso=setpoint-output;
	MOVF        R0, 0 
	SUBWF       _setpoint+0, 0 
	MOVWF       R0 
	MOVF        R1, 0 
	SUBWFB      _setpoint+1, 0 
	MOVWF       R1 
	CALL        _int2double+0, 0
	MOVF        R0, 0 
	MOVWF       _saiso+0 
	MOVF        R1, 0 
	MOVWF       _saiso+1 
	MOVF        R2, 0 
	MOVWF       _saiso+2 
	MOVF        R3, 0 
	MOVWF       _saiso+3 
;MyProjectPIDPIC18F.c,67 :: 		saiso_i=saiso_i+saiso*0.04;
	MOVLW       10
	MOVWF       R4 
	MOVLW       215
	MOVWF       R5 
	MOVLW       35
	MOVWF       R6 
	MOVLW       122
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVF        _saiso_i+0, 0 
	MOVWF       R4 
	MOVF        _saiso_i+1, 0 
	MOVWF       R5 
	MOVF        _saiso_i+2, 0 
	MOVWF       R6 
	MOVF        _saiso_i+3, 0 
	MOVWF       R7 
	CALL        _Add_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       _saiso_i+0 
	MOVF        R1, 0 
	MOVWF       _saiso_i+1 
	MOVF        R2, 0 
	MOVWF       _saiso_i+2 
	MOVF        R3, 0 
	MOVWF       _saiso_i+3 
;MyProjectPIDPIC18F.c,68 :: 		if (saiso_i>=100) saiso_i=100;
	MOVLW       0
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVLW       72
	MOVWF       R6 
	MOVLW       133
	MOVWF       R7 
	CALL        _Compare_Double+0, 0
	MOVLW       0
	BTFSC       STATUS+0, 0 
	MOVLW       1
	MOVWF       R0 
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_ngat1
	MOVLW       0
	MOVWF       _saiso_i+0 
	MOVLW       0
	MOVWF       _saiso_i+1 
	MOVLW       72
	MOVWF       _saiso_i+2 
	MOVLW       133
	MOVWF       _saiso_i+3 
L_ngat1:
;MyProjectPIDPIC18F.c,69 :: 		if (saiso_i<=-100) saiso_i=-100;
	MOVF        _saiso_i+0, 0 
	MOVWF       R4 
	MOVF        _saiso_i+1, 0 
	MOVWF       R5 
	MOVF        _saiso_i+2, 0 
	MOVWF       R6 
	MOVF        _saiso_i+3, 0 
	MOVWF       R7 
	MOVLW       0
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVLW       200
	MOVWF       R2 
	MOVLW       133
	MOVWF       R3 
	CALL        _Compare_Double+0, 0
	MOVLW       0
	BTFSC       STATUS+0, 0 
	MOVLW       1
	MOVWF       R0 
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_ngat2
	MOVLW       0
	MOVWF       _saiso_i+0 
	MOVLW       0
	MOVWF       _saiso_i+1 
	MOVLW       200
	MOVWF       _saiso_i+2 
	MOVLW       133
	MOVWF       _saiso_i+3 
L_ngat2:
;MyProjectPIDPIC18F.c,70 :: 		saiso_d=(saiso - saiso_tmp)*25;
	MOVF        _saiso_tmp+0, 0 
	MOVWF       R4 
	MOVF        _saiso_tmp+1, 0 
	MOVWF       R5 
	MOVF        _saiso_tmp+2, 0 
	MOVWF       R6 
	MOVF        _saiso_tmp+3, 0 
	MOVWF       R7 
	MOVF        _saiso+0, 0 
	MOVWF       R0 
	MOVF        _saiso+1, 0 
	MOVWF       R1 
	MOVF        _saiso+2, 0 
	MOVWF       R2 
	MOVF        _saiso+3, 0 
	MOVWF       R3 
	CALL        _Sub_32x32_FP+0, 0
	MOVLW       0
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVLW       72
	MOVWF       R6 
	MOVLW       131
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       FLOC__ngat+4 
	MOVF        R1, 0 
	MOVWF       FLOC__ngat+5 
	MOVF        R2, 0 
	MOVWF       FLOC__ngat+6 
	MOVF        R3, 0 
	MOVWF       FLOC__ngat+7 
	MOVF        FLOC__ngat+4, 0 
	MOVWF       _saiso_d+0 
	MOVF        FLOC__ngat+5, 0 
	MOVWF       _saiso_d+1 
	MOVF        FLOC__ngat+6, 0 
	MOVWF       _saiso_d+2 
	MOVF        FLOC__ngat+7, 0 
	MOVWF       _saiso_d+3 
;MyProjectPIDPIC18F.c,71 :: 		duty= duty+saiso*Kp+saiso_i*Ki+saiso_d*Kd;
	MOVF        _saiso+0, 0 
	MOVWF       R0 
	MOVF        _saiso+1, 0 
	MOVWF       R1 
	MOVF        _saiso+2, 0 
	MOVWF       R2 
	MOVF        _saiso+3, 0 
	MOVWF       R3 
	MOVF        _Kp+0, 0 
	MOVWF       R4 
	MOVF        _Kp+1, 0 
	MOVWF       R5 
	MOVF        _Kp+2, 0 
	MOVWF       R6 
	MOVF        _Kp+3, 0 
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVF        _duty+0, 0 
	MOVWF       R4 
	MOVF        _duty+1, 0 
	MOVWF       R5 
	MOVF        _duty+2, 0 
	MOVWF       R6 
	MOVF        _duty+3, 0 
	MOVWF       R7 
	CALL        _Add_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       FLOC__ngat+0 
	MOVF        R1, 0 
	MOVWF       FLOC__ngat+1 
	MOVF        R2, 0 
	MOVWF       FLOC__ngat+2 
	MOVF        R3, 0 
	MOVWF       FLOC__ngat+3 
	MOVF        _saiso_i+0, 0 
	MOVWF       R0 
	MOVF        _saiso_i+1, 0 
	MOVWF       R1 
	MOVF        _saiso_i+2, 0 
	MOVWF       R2 
	MOVF        _saiso_i+3, 0 
	MOVWF       R3 
	MOVF        _Ki+0, 0 
	MOVWF       R4 
	MOVF        _Ki+1, 0 
	MOVWF       R5 
	MOVF        _Ki+2, 0 
	MOVWF       R6 
	MOVF        _Ki+3, 0 
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVF        FLOC__ngat+0, 0 
	MOVWF       R4 
	MOVF        FLOC__ngat+1, 0 
	MOVWF       R5 
	MOVF        FLOC__ngat+2, 0 
	MOVWF       R6 
	MOVF        FLOC__ngat+3, 0 
	MOVWF       R7 
	CALL        _Add_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       FLOC__ngat+0 
	MOVF        R1, 0 
	MOVWF       FLOC__ngat+1 
	MOVF        R2, 0 
	MOVWF       FLOC__ngat+2 
	MOVF        R3, 0 
	MOVWF       FLOC__ngat+3 
	MOVF        FLOC__ngat+4, 0 
	MOVWF       R0 
	MOVF        FLOC__ngat+5, 0 
	MOVWF       R1 
	MOVF        FLOC__ngat+6, 0 
	MOVWF       R2 
	MOVF        FLOC__ngat+7, 0 
	MOVWF       R3 
	MOVF        _Kd+0, 0 
	MOVWF       R4 
	MOVF        _Kd+1, 0 
	MOVWF       R5 
	MOVF        _Kd+2, 0 
	MOVWF       R6 
	MOVF        _Kd+3, 0 
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVF        FLOC__ngat+0, 0 
	MOVWF       R4 
	MOVF        FLOC__ngat+1, 0 
	MOVWF       R5 
	MOVF        FLOC__ngat+2, 0 
	MOVWF       R6 
	MOVF        FLOC__ngat+3, 0 
	MOVWF       R7 
	CALL        _Add_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       _duty+0 
	MOVF        R1, 0 
	MOVWF       _duty+1 
	MOVF        R2, 0 
	MOVWF       _duty+2 
	MOVF        R3, 0 
	MOVWF       _duty+3 
;MyProjectPIDPIC18F.c,72 :: 		if (duty>=255) duty=255;
	MOVLW       0
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVLW       127
	MOVWF       R6 
	MOVLW       134
	MOVWF       R7 
	CALL        _Compare_Double+0, 0
	MOVLW       0
	BTFSC       STATUS+0, 0 
	MOVLW       1
	MOVWF       R0 
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_ngat3
	MOVLW       0
	MOVWF       _duty+0 
	MOVLW       0
	MOVWF       _duty+1 
	MOVLW       127
	MOVWF       _duty+2 
	MOVLW       134
	MOVWF       _duty+3 
L_ngat3:
;MyProjectPIDPIC18F.c,73 :: 		if (duty<=0) duty=0;
	MOVF        _duty+0, 0 
	MOVWF       R4 
	MOVF        _duty+1, 0 
	MOVWF       R5 
	MOVF        _duty+2, 0 
	MOVWF       R6 
	MOVF        _duty+3, 0 
	MOVWF       R7 
	CLRF        R0 
	CLRF        R1 
	CLRF        R2 
	CLRF        R3 
	CALL        _Compare_Double+0, 0
	MOVLW       0
	BTFSC       STATUS+0, 0 
	MOVLW       1
	MOVWF       R0 
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_ngat4
	CLRF        _duty+0 
	CLRF        _duty+1 
	CLRF        _duty+2 
	CLRF        _duty+3 
L_ngat4:
;MyProjectPIDPIC18F.c,74 :: 		saiso_tmp=saiso;
	MOVF        _saiso+0, 0 
	MOVWF       _saiso_tmp+0 
	MOVF        _saiso+1, 0 
	MOVWF       _saiso_tmp+1 
	MOVF        _saiso+2, 0 
	MOVWF       _saiso_tmp+2 
	MOVF        _saiso+3, 0 
	MOVWF       _saiso_tmp+3 
;MyProjectPIDPIC18F.c,75 :: 		}
L_ngat0:
;MyProjectPIDPIC18F.c,78 :: 		if (INT0IF_bit==1)
	BTFSS       INT0IF_bit+0, BitPos(INT0IF_bit+0) 
	GOTO        L_ngat5
;MyProjectPIDPIC18F.c,80 :: 		xung++;
	INFSNZ      _xung+0, 1 
	INCF        _xung+1, 1 
;MyProjectPIDPIC18F.c,81 :: 		INT0IF_bit=0;
	BCF         INT0IF_bit+0, BitPos(INT0IF_bit+0) 
;MyProjectPIDPIC18F.c,82 :: 		}
L_ngat5:
;MyProjectPIDPIC18F.c,83 :: 		}
L_end_ngat:
L__ngat81:
	RETFIE      1
; end of _ngat

_ngatthap:
	MOVWF       ___Low_saveWREG+0 
	MOVF        STATUS+0, 0 
	MOVWF       ___Low_saveSTATUS+0 
	MOVF        BSR+0, 0 
	MOVWF       ___Low_saveBSR+0 

;MyProjectPIDPIC18F.c,84 :: 		void ngatthap()iv 0x18
;MyProjectPIDPIC18F.c,85 :: 		{if(rcif_bit==1)
	BTFSS       RCIF_bit+0, BitPos(RCIF_bit+0) 
	GOTO        L_ngatthap6
;MyProjectPIDPIC18F.c,86 :: 		{rcif_bit=0;
	BCF         RCIF_bit+0, BitPos(RCIF_bit+0) 
;MyProjectPIDPIC18F.c,88 :: 		v=uart1_read();
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _v+0 
;MyProjectPIDPIC18F.c,89 :: 		switch(v)
	GOTO        L_ngatthap7
;MyProjectPIDPIC18F.c,91 :: 		case 255:
L_ngatthap9:
;MyProjectPIDPIC18F.c,92 :: 		number=15;//lenh stop
	MOVLW       15
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,93 :: 		break;
	GOTO        L_ngatthap8
;MyProjectPIDPIC18F.c,94 :: 		case 254:
L_ngatthap10:
;MyProjectPIDPIC18F.c,95 :: 		number=16;// lenh start
	MOVLW       16
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,96 :: 		break;
	GOTO        L_ngatthap8
;MyProjectPIDPIC18F.c,97 :: 		case 253:// dk su dug may tinh
L_ngatthap11:
;MyProjectPIDPIC18F.c,98 :: 		dk=1;
	MOVLW       1
	MOVWF       _dk+0 
;MyProjectPIDPIC18F.c,99 :: 		break;
	GOTO        L_ngatthap8
;MyProjectPIDPIC18F.c,100 :: 		case 252://lenh su dung ban fim
L_ngatthap12:
;MyProjectPIDPIC18F.c,101 :: 		dk=0;
	CLRF        _dk+0 
;MyProjectPIDPIC18F.c,102 :: 		break;
	GOTO        L_ngatthap8
;MyProjectPIDPIC18F.c,103 :: 		default:
L_ngatthap13:
;MyProjectPIDPIC18F.c,104 :: 		setpoint=v;
	MOVF        _v+0, 0 
	MOVWF       _setpoint+0 
	MOVLW       0
	MOVWF       _setpoint+1 
;MyProjectPIDPIC18F.c,105 :: 		set=1;
	MOVLW       1
	MOVWF       _set+0 
;MyProjectPIDPIC18F.c,106 :: 		number=12;
	MOVLW       12
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,108 :: 		}
	GOTO        L_ngatthap8
L_ngatthap7:
	MOVF        _v+0, 0 
	XORLW       255
	BTFSC       STATUS+0, 2 
	GOTO        L_ngatthap9
	MOVF        _v+0, 0 
	XORLW       254
	BTFSC       STATUS+0, 2 
	GOTO        L_ngatthap10
	MOVF        _v+0, 0 
	XORLW       253
	BTFSC       STATUS+0, 2 
	GOTO        L_ngatthap11
	MOVF        _v+0, 0 
	XORLW       252
	BTFSC       STATUS+0, 2 
	GOTO        L_ngatthap12
	GOTO        L_ngatthap13
L_ngatthap8:
;MyProjectPIDPIC18F.c,109 :: 		}
L_ngatthap6:
;MyProjectPIDPIC18F.c,110 :: 		}
L_end_ngatthap:
L__ngatthap83:
	MOVF        ___Low_saveBSR+0, 0 
	MOVWF       BSR+0 
	MOVF        ___Low_saveSTATUS+0, 0 
	MOVWF       STATUS+0 
	SWAPF       ___Low_saveWREG+0, 1 
	SWAPF       ___Low_saveWREG+0, 0 
	RETFIE      0
; end of _ngatthap

_chongdoi:

;MyProjectPIDPIC18F.c,112 :: 		void chongdoi()
;MyProjectPIDPIC18F.c,114 :: 		doinhan:
___chongdoi_doinhan:
;MyProjectPIDPIC18F.c,115 :: 		for(i=0;i<2500;i++)
	CLRF        _i+0 
	CLRF        _i+1 
L_chongdoi14:
	MOVLW       128
	XORWF       _i+1, 0 
	MOVWF       R0 
	MOVLW       128
	XORLW       9
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__chongdoi85
	MOVLW       196
	SUBWF       _i+0, 0 
L__chongdoi85:
	BTFSC       STATUS+0, 0 
	GOTO        L_chongdoi15
;MyProjectPIDPIC18F.c,117 :: 		if(cot==0xf0)//0b11110000
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSS       STATUS+0, 2 
	GOTO        L_chongdoi17
;MyProjectPIDPIC18F.c,118 :: 		goto doinhan;
	GOTO        ___chongdoi_doinhan
L_chongdoi17:
;MyProjectPIDPIC18F.c,115 :: 		for(i=0;i<2500;i++)
	INFSNZ      _i+0, 1 
	INCF        _i+1, 1 
;MyProjectPIDPIC18F.c,119 :: 		}
	GOTO        L_chongdoi14
L_chongdoi15:
;MyProjectPIDPIC18F.c,120 :: 		maphim=cot;
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       _maphim+0 
;MyProjectPIDPIC18F.c,121 :: 		while(cot!=0xf0){}
L_chongdoi18:
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSC       STATUS+0, 2 
	GOTO        L_chongdoi19
	GOTO        L_chongdoi18
L_chongdoi19:
;MyProjectPIDPIC18F.c,122 :: 		doinha:
___chongdoi_doinha:
;MyProjectPIDPIC18F.c,123 :: 		for(i=0;i<2500;i++)
	CLRF        _i+0 
	CLRF        _i+1 
L_chongdoi20:
	MOVLW       128
	XORWF       _i+1, 0 
	MOVWF       R0 
	MOVLW       128
	XORLW       9
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__chongdoi86
	MOVLW       196
	SUBWF       _i+0, 0 
L__chongdoi86:
	BTFSC       STATUS+0, 0 
	GOTO        L_chongdoi21
;MyProjectPIDPIC18F.c,125 :: 		if(cot!=0xf0)
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSC       STATUS+0, 2 
	GOTO        L_chongdoi23
;MyProjectPIDPIC18F.c,126 :: 		goto doinha;
	GOTO        ___chongdoi_doinha
L_chongdoi23:
;MyProjectPIDPIC18F.c,123 :: 		for(i=0;i<2500;i++)
	INFSNZ      _i+0, 1 
	INCF        _i+1, 1 
;MyProjectPIDPIC18F.c,127 :: 		}
	GOTO        L_chongdoi20
L_chongdoi21:
;MyProjectPIDPIC18F.c,128 :: 		}
L_end_chongdoi:
	RETURN      0
; end of _chongdoi

_quetphim:

;MyProjectPIDPIC18F.c,130 :: 		void quetphim()
;MyProjectPIDPIC18F.c,132 :: 		latd3_bit=1;//cot4=1
	BSF         LATD3_bit+0, BitPos(LATD3_bit+0) 
;MyProjectPIDPIC18F.c,133 :: 		latd0_bit=0;//cot1=0
	BCF         LATD0_bit+0, BitPos(LATD0_bit+0) 
;MyProjectPIDPIC18F.c,134 :: 		if(cot!=0xf0)
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim24
;MyProjectPIDPIC18F.c,136 :: 		chongdoi();
	CALL        _chongdoi+0, 0
;MyProjectPIDPIC18F.c,137 :: 		maphim= (maphim|0x01);
	BSF         _maphim+0, 0 
;MyProjectPIDPIC18F.c,138 :: 		}
L_quetphim24:
;MyProjectPIDPIC18F.c,140 :: 		latd0_bit=1;//cot1=1
	BSF         LATD0_bit+0, BitPos(LATD0_bit+0) 
;MyProjectPIDPIC18F.c,141 :: 		latd1_bit=0;//cot2=0
	BCF         LATD1_bit+0, BitPos(LATD1_bit+0) 
;MyProjectPIDPIC18F.c,142 :: 		if(cot!=0xf0)
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim25
;MyProjectPIDPIC18F.c,144 :: 		chongdoi();
	CALL        _chongdoi+0, 0
;MyProjectPIDPIC18F.c,145 :: 		maphim= (maphim|0x02);
	BSF         _maphim+0, 1 
;MyProjectPIDPIC18F.c,146 :: 		}
L_quetphim25:
;MyProjectPIDPIC18F.c,148 :: 		latd1_bit=1;//cot2=1
	BSF         LATD1_bit+0, BitPos(LATD1_bit+0) 
;MyProjectPIDPIC18F.c,149 :: 		latd2_bit=0;//cot3=0
	BCF         LATD2_bit+0, BitPos(LATD2_bit+0) 
;MyProjectPIDPIC18F.c,150 :: 		if(cot!=0xf0)
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim26
;MyProjectPIDPIC18F.c,152 :: 		chongdoi();
	CALL        _chongdoi+0, 0
;MyProjectPIDPIC18F.c,153 :: 		maphim= (maphim|0x04);
	BSF         _maphim+0, 2 
;MyProjectPIDPIC18F.c,154 :: 		}
L_quetphim26:
;MyProjectPIDPIC18F.c,156 :: 		latd2_bit=1;//cot1=1
	BSF         LATD2_bit+0, BitPos(LATD2_bit+0) 
;MyProjectPIDPIC18F.c,157 :: 		latd3_bit=0;//cot4=0
	BCF         LATD3_bit+0, BitPos(LATD3_bit+0) 
;MyProjectPIDPIC18F.c,158 :: 		if(cot!=0xf0)
	MOVLW       240
	ANDWF       PORTD+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       240
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim27
;MyProjectPIDPIC18F.c,160 :: 		chongdoi();
	CALL        _chongdoi+0, 0
;MyProjectPIDPIC18F.c,161 :: 		maphim= (maphim|0x08);
	BSF         _maphim+0, 3 
;MyProjectPIDPIC18F.c,162 :: 		}
L_quetphim27:
;MyProjectPIDPIC18F.c,163 :: 		switch(maphim)
	GOTO        L_quetphim28
;MyProjectPIDPIC18F.c,165 :: 		case 0xE1:
L_quetphim30:
;MyProjectPIDPIC18F.c,166 :: 		number=1;
	MOVLW       1
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,167 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,168 :: 		case 0xE2:
L_quetphim31:
;MyProjectPIDPIC18F.c,169 :: 		number=2;
	MOVLW       2
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,170 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,171 :: 		case 0xE4:
L_quetphim32:
;MyProjectPIDPIC18F.c,172 :: 		number=3;
	MOVLW       3
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,173 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,174 :: 		case 0xE8:
L_quetphim33:
;MyProjectPIDPIC18F.c,175 :: 		number=4;
	MOVLW       4
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,176 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,177 :: 		case 0xD1:
L_quetphim34:
;MyProjectPIDPIC18F.c,178 :: 		number=5;
	MOVLW       5
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,179 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,180 :: 		case 0xD2:
L_quetphim35:
;MyProjectPIDPIC18F.c,181 :: 		number=6;
	MOVLW       6
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,182 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,183 :: 		case 0xD4:
L_quetphim36:
;MyProjectPIDPIC18F.c,184 :: 		number=7;
	MOVLW       7
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,185 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,186 :: 		case 0xD8:
L_quetphim37:
;MyProjectPIDPIC18F.c,187 :: 		number=8;
	MOVLW       8
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,188 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,189 :: 		case 0xB1:
L_quetphim38:
;MyProjectPIDPIC18F.c,190 :: 		number=9;
	MOVLW       9
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,191 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,192 :: 		case 0xB2:
L_quetphim39:
;MyProjectPIDPIC18F.c,193 :: 		number=10;
	MOVLW       10
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,194 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,195 :: 		case 0xB4:
L_quetphim40:
;MyProjectPIDPIC18F.c,196 :: 		number=11;
	MOVLW       11
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,197 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,198 :: 		case 0xB8:
L_quetphim41:
;MyProjectPIDPIC18F.c,199 :: 		number=12;
	MOVLW       12
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,200 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,201 :: 		case 0x71:
L_quetphim42:
;MyProjectPIDPIC18F.c,202 :: 		number=13;
	MOVLW       13
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,203 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,204 :: 		case 0x72:
L_quetphim43:
;MyProjectPIDPIC18F.c,205 :: 		number=14;
	MOVLW       14
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,206 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,207 :: 		case 0x74:
L_quetphim44:
;MyProjectPIDPIC18F.c,208 :: 		number=15;
	MOVLW       15
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,209 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,210 :: 		case 0x78:
L_quetphim45:
;MyProjectPIDPIC18F.c,211 :: 		number=16;
	MOVLW       16
	MOVWF       _number+0 
;MyProjectPIDPIC18F.c,212 :: 		break;
	GOTO        L_quetphim29
;MyProjectPIDPIC18F.c,213 :: 		}
L_quetphim28:
	MOVF        _maphim+0, 0 
	XORLW       225
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim30
	MOVF        _maphim+0, 0 
	XORLW       226
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim31
	MOVF        _maphim+0, 0 
	XORLW       228
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim32
	MOVF        _maphim+0, 0 
	XORLW       232
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim33
	MOVF        _maphim+0, 0 
	XORLW       209
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim34
	MOVF        _maphim+0, 0 
	XORLW       210
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim35
	MOVF        _maphim+0, 0 
	XORLW       212
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim36
	MOVF        _maphim+0, 0 
	XORLW       216
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim37
	MOVF        _maphim+0, 0 
	XORLW       177
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim38
	MOVF        _maphim+0, 0 
	XORLW       178
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim39
	MOVF        _maphim+0, 0 
	XORLW       180
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim40
	MOVF        _maphim+0, 0 
	XORLW       184
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim41
	MOVF        _maphim+0, 0 
	XORLW       113
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim42
	MOVF        _maphim+0, 0 
	XORLW       114
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim43
	MOVF        _maphim+0, 0 
	XORLW       116
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim44
	MOVF        _maphim+0, 0 
	XORLW       120
	BTFSC       STATUS+0, 2 
	GOTO        L_quetphim45
L_quetphim29:
;MyProjectPIDPIC18F.c,214 :: 		maphim=0;
	CLRF        _maphim+0 
;MyProjectPIDPIC18F.c,215 :: 		}
L_end_quetphim:
	RETURN      0
; end of _quetphim

_setup:

;MyProjectPIDPIC18F.c,217 :: 		void setup()
;MyProjectPIDPIC18F.c,219 :: 		bytetostr(mang[0],txt1);
	MOVF        _mang+0, 0 
	MOVWF       FARG_ByteToStr_input+0 
	MOVLW       _txt1+0
	MOVWF       FARG_ByteToStr_output+0 
	MOVLW       hi_addr(_txt1+0)
	MOVWF       FARG_ByteToStr_output+1 
	CALL        _ByteToStr+0, 0
;MyProjectPIDPIC18F.c,220 :: 		bytetostr(mang[1],txt2);
	MOVF        _mang+2, 0 
	MOVWF       FARG_ByteToStr_input+0 
	MOVLW       _txt2+0
	MOVWF       FARG_ByteToStr_output+0 
	MOVLW       hi_addr(_txt2+0)
	MOVWF       FARG_ByteToStr_output+1 
	CALL        _ByteToStr+0, 0
;MyProjectPIDPIC18F.c,221 :: 		bytetostr(mang[2],txt3);
	MOVF        _mang+4, 0 
	MOVWF       FARG_ByteToStr_input+0 
	MOVLW       _txt3+0
	MOVWF       FARG_ByteToStr_output+0 
	MOVLW       hi_addr(_txt3+0)
	MOVWF       FARG_ByteToStr_output+1 
	CALL        _ByteToStr+0, 0
;MyProjectPIDPIC18F.c,223 :: 		lcd_out(1,4,txt1);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       4
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _txt1+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_txt1+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,224 :: 		lcd_out(1,8,txt2);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       8
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _txt2+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_txt2+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,225 :: 		lcd_out(1,12,txt3);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       12
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _txt3+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_txt3+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,227 :: 		lcd_out(1,1,"NHAP:");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr1_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr1_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,229 :: 		number=0;
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,230 :: 		}
L_end_setup:
	RETURN      0
; end of _setup

_ok:

;MyProjectPIDPIC18F.c,232 :: 		void ok()
;MyProjectPIDPIC18F.c,234 :: 		lcd_cmd(_lcd_clear);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,235 :: 		if(dk==0)
	MOVF        _dk+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_ok46
;MyProjectPIDPIC18F.c,236 :: 		setpoint=(mang[0]*100+mang[1]*10+mang[2]);
	MOVF        _mang+0, 0 
	MOVWF       R0 
	MOVF        _mang+1, 0 
	MOVWF       R1 
	MOVLW       100
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVF        R0, 0 
	MOVWF       FLOC__ok+0 
	MOVF        R1, 0 
	MOVWF       FLOC__ok+1 
	MOVF        _mang+2, 0 
	MOVWF       R0 
	MOVF        _mang+3, 0 
	MOVWF       R1 
	MOVLW       10
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVF        R0, 0 
	ADDWF       FLOC__ok+0, 0 
	MOVWF       _setpoint+0 
	MOVF        R1, 0 
	ADDWFC      FLOC__ok+1, 0 
	MOVWF       _setpoint+1 
	MOVF        _mang+4, 0 
	ADDWF       _setpoint+0, 1 
	MOVF        _mang+5, 0 
	ADDWFC      _setpoint+1, 1 
L_ok46:
;MyProjectPIDPIC18F.c,238 :: 		if(setpoint>100)
	MOVLW       128
	MOVWF       R0 
	MOVLW       128
	XORWF       _setpoint+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__ok90
	MOVF        _setpoint+0, 0 
	SUBLW       100
L__ok90:
	BTFSC       STATUS+0, 0 
	GOTO        L_ok47
;MyProjectPIDPIC18F.c,240 :: 		setpoint=100;
	MOVLW       100
	MOVWF       _setpoint+0 
	MOVLW       0
	MOVWF       _setpoint+1 
;MyProjectPIDPIC18F.c,241 :: 		lcd_out(1,1,"TOI DA 100M/P");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr2_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr2_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,242 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_ok48:
	DECFSZ      R13, 1, 1
	BRA         L_ok48
	DECFSZ      R12, 1, 1
	BRA         L_ok48
	DECFSZ      R11, 1, 1
	BRA         L_ok48
	NOP
;MyProjectPIDPIC18F.c,243 :: 		}
L_ok47:
;MyProjectPIDPIC18F.c,245 :: 		lcd_cmd(_lcd_clear);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,246 :: 		wordtostr(setpoint,txt6);
	MOVF        _setpoint+0, 0 
	MOVWF       FARG_WordToStr_input+0 
	MOVF        _setpoint+1, 0 
	MOVWF       FARG_WordToStr_input+1 
	MOVLW       _txt6+0
	MOVWF       FARG_WordToStr_output+0 
	MOVLW       hi_addr(_txt6+0)
	MOVWF       FARG_WordToStr_output+1 
	CALL        _WordToStr+0, 0
;MyProjectPIDPIC18F.c,248 :: 		lcd_out(1,0,txt6);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	CLRF        FARG_Lcd_Out_column+0 
	MOVLW       _txt6+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_txt6+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,250 :: 		lcd_out(2,1,"Tocdo:");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr3_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr3_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,251 :: 		lcd_out(2,14,"M/p");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       14
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr4_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr4_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,252 :: 		lcd_out(1,7,"SETUP");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       7
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr5_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr5_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,253 :: 		for(k=0;k<j;k++)//--**--xoa toan bo mang sau khi da nhap xong--**--
	CLRF        _k+0 
L_ok49:
	MOVF        _j+0, 0 
	SUBWF       _k+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_ok50
;MyProjectPIDPIC18F.c,254 :: 		{mang[k]=0;
	MOVF        _k+0, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVLW       _mang+0
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       hi_addr(_mang+0)
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	CLRF        POSTINC1+0 
	CLRF        POSTINC1+0 
;MyProjectPIDPIC18F.c,253 :: 		for(k=0;k<j;k++)//--**--xoa toan bo mang sau khi da nhap xong--**--
	INCF        _k+0, 1 
;MyProjectPIDPIC18F.c,256 :: 		}
	GOTO        L_ok49
L_ok50:
;MyProjectPIDPIC18F.c,257 :: 		j=0;
	CLRF        _j+0 
;MyProjectPIDPIC18F.c,258 :: 		number=0;
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,259 :: 		}
L_end_ok:
	RETURN      0
; end of _ok

_start:

;MyProjectPIDPIC18F.c,261 :: 		void start()
;MyProjectPIDPIC18F.c,263 :: 		setpoint=0;
	CLRF        _setpoint+0 
	CLRF        _setpoint+1 
;MyProjectPIDPIC18F.c,264 :: 		lcd_cmd(_lcd_clear);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,265 :: 		pwm1_start();
	CALL        _PWM1_Start+0, 0
;MyProjectPIDPIC18F.c,266 :: 		pwm1_set_duty(duty);
	MOVF        _duty+0, 0 
	MOVWF       R0 
	MOVF        _duty+1, 0 
	MOVWF       R1 
	MOVF        _duty+2, 0 
	MOVWF       R2 
	MOVF        _duty+3, 0 
	MOVWF       R3 
	CALL        _double2byte+0, 0
	MOVF        R0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;MyProjectPIDPIC18F.c,267 :: 		lcd_out(2,1,"Tocdo:");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr6_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr6_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,268 :: 		lcd_out(2,14,"M/P");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       14
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr7_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr7_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,269 :: 		lcd_out(1,7,"SETUP");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       7
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr8_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr8_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,270 :: 		number=0;
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,271 :: 		}
L_end_start:
	RETURN      0
; end of _start

_stop:

;MyProjectPIDPIC18F.c,272 :: 		void stop()
;MyProjectPIDPIC18F.c,275 :: 		setpoint=0;
	CLRF        _setpoint+0 
	CLRF        _setpoint+1 
;MyProjectPIDPIC18F.c,276 :: 		pwm1_start();
	CALL        _PWM1_Start+0, 0
;MyProjectPIDPIC18F.c,277 :: 		lcd_cmd(_lcd_clear);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,278 :: 		lcd_out(1,7,"STOPPED");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       7
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr9_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr9_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,279 :: 		number=0;
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,280 :: 		}
L_end_stop:
	RETURN      0
; end of _stop

_cancel:

;MyProjectPIDPIC18F.c,281 :: 		void cancel()
;MyProjectPIDPIC18F.c,283 :: 		lcd_cmd(_lcd_clear);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,284 :: 		pwm1_set_duty(duty);
	MOVF        _duty+0, 0 
	MOVWF       R0 
	MOVF        _duty+1, 0 
	MOVWF       R1 
	MOVF        _duty+2, 0 
	MOVWF       R2 
	MOVF        _duty+3, 0 
	MOVWF       R3 
	CALL        _double2byte+0, 0
	MOVF        R0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;MyProjectPIDPIC18F.c,285 :: 		lcd_out(2,1,"Tocdo:");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr10_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr10_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,286 :: 		lcd_out(2,14,"M/P");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       14
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr11_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr11_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,288 :: 		lcd_out(1,7,"SETUP");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       7
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr12_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr12_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,289 :: 		number=0;
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,290 :: 		}
L_end_cancel:
	RETURN      0
; end of _cancel

_main:

;MyProjectPIDPIC18F.c,292 :: 		void main() {
;MyProjectPIDPIC18F.c,293 :: 		ADCON1= 0X0F;                   // Configure AN pins as digital I/O
	MOVLW       15
	MOVWF       ADCON1+0 
;MyProjectPIDPIC18F.c,294 :: 		trisd=0xf0;latd=0x0f;//port quet phim ma tran
	MOVLW       240
	MOVWF       TRISD+0 
	MOVLW       15
	MOVWF       LATD+0 
;MyProjectPIDPIC18F.c,295 :: 		trisa=0x00;// ngo ra lcd
	CLRF        TRISA+0 
;MyProjectPIDPIC18F.c,296 :: 		trise=0x00;
	CLRF        TRISE+0 
;MyProjectPIDPIC18F.c,297 :: 		lata1_bit=0;//--**chan rewrite cua lcd--**--
	BCF         LATA1_bit+0, BitPos(LATA1_bit+0) 
;MyProjectPIDPIC18F.c,298 :: 		TRISC2_BIT=0;
	BCF         TRISC2_bit+0, BitPos(TRISC2_bit+0) 
;MyProjectPIDPIC18F.c,299 :: 		TRISC6_BIT=0;
	BCF         TRISC6_bit+0, BitPos(TRISC6_bit+0) 
;MyProjectPIDPIC18F.c,300 :: 		TRISC7_BIT=1;
	BSF         TRISC7_bit+0, BitPos(TRISC7_bit+0) 
;MyProjectPIDPIC18F.c,302 :: 		trisB.F1= 0;
	BCF         TRISB+0, 1 
;MyProjectPIDPIC18F.c,303 :: 		latB.f1=0; //chan DIR
	BCF         LATB+0, 1 
;MyProjectPIDPIC18F.c,304 :: 		TRISB.F0=1;//chan doc xung encoder
	BSF         TRISB+0, 0 
;MyProjectPIDPIC18F.c,306 :: 		GIE_bit=1;
	BSF         GIE_bit+0, BitPos(GIE_bit+0) 
;MyProjectPIDPIC18F.c,307 :: 		PEIE_bit=1;
	BSF         PEIE_bit+0, BitPos(PEIE_bit+0) 
;MyProjectPIDPIC18F.c,308 :: 		ipen_bit=1;
	BSF         IPEN_bit+0, BitPos(IPEN_bit+0) 
;MyProjectPIDPIC18F.c,309 :: 		rcie_bit=1;rcip_bit=0;
	BSF         RCIE_bit+0, BitPos(RCIE_bit+0) 
	BCF         RCIP_bit+0, BitPos(RCIP_bit+0) 
;MyProjectPIDPIC18F.c,310 :: 		rcif_bit=0;
	BCF         RCIF_bit+0, BitPos(RCIF_bit+0) 
;MyProjectPIDPIC18F.c,313 :: 		T08BIT_bit=0;
	BCF         T08BIT_bit+0, BitPos(T08BIT_bit+0) 
;MyProjectPIDPIC18F.c,314 :: 		PSA_bit=0;
	BCF         PSA_bit+0, BitPos(PSA_bit+0) 
;MyProjectPIDPIC18F.c,315 :: 		TMR0IP_bit=1;
	BSF         TMR0IP_bit+0, BitPos(TMR0IP_bit+0) 
;MyProjectPIDPIC18F.c,317 :: 		T0CS_bit=0;
	BCF         T0CS_bit+0, BitPos(T0CS_bit+0) 
;MyProjectPIDPIC18F.c,318 :: 		T0PS0_bit=1;
	BSF         T0PS0_bit+0, BitPos(T0PS0_bit+0) 
;MyProjectPIDPIC18F.c,319 :: 		T0PS1_bit=0; // Prescale 1:4
	BCF         T0PS1_bit+0, BitPos(T0PS1_bit+0) 
;MyProjectPIDPIC18F.c,320 :: 		T0PS2_bit=0;
	BCF         T0PS2_bit+0, BitPos(T0PS2_bit+0) 
;MyProjectPIDPIC18F.c,321 :: 		TMR0L=0xB0;
	MOVLW       176
	MOVWF       TMR0L+0 
;MyProjectPIDPIC18F.c,322 :: 		TMR0H=0x3C;
	MOVLW       60
	MOVWF       TMR0H+0 
;MyProjectPIDPIC18F.c,323 :: 		TMR0IF_bit=0;
	BCF         TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
;MyProjectPIDPIC18F.c,324 :: 		INT0IF_bit=0;
	BCF         INT0IF_bit+0, BitPos(INT0IF_bit+0) 
;MyProjectPIDPIC18F.c,325 :: 		TMR0IE_bit=1;
	BSF         TMR0IE_bit+0, BitPos(TMR0IE_bit+0) 
;MyProjectPIDPIC18F.c,326 :: 		INTEDG0_bit = 1;
	BSF         INTEDG0_bit+0, BitPos(INTEDG0_bit+0) 
;MyProjectPIDPIC18F.c,327 :: 		INT0IE_bit=1;
	BSF         INT0IE_bit+0, BitPos(INT0IE_bit+0) 
;MyProjectPIDPIC18F.c,328 :: 		TMR0ON_bit=1;
	BSF         TMR0ON_bit+0, BitPos(TMR0ON_bit+0) 
;MyProjectPIDPIC18F.c,329 :: 		dk=0;
	CLRF        _dk+0 
;MyProjectPIDPIC18F.c,330 :: 		setpoint=0;
	CLRF        _setpoint+0 
	CLRF        _setpoint+1 
;MyProjectPIDPIC18F.c,331 :: 		pwm1_stop();
	CALL        _PWM1_Stop+0, 0
;MyProjectPIDPIC18F.c,333 :: 		UART1_Init(9600);
	BSF         BAUDCON+0, 3, 0
	MOVLW       2
	MOVWF       SPBRGH+0 
	MOVLW       8
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
;MyProjectPIDPIC18F.c,334 :: 		PWM1_Init(19000);
	BSF         T2CON+0, 0, 0
	BCF         T2CON+0, 1, 0
	MOVLW       65
	MOVWF       PR2+0, 0
	CALL        _PWM1_Init+0, 0
;MyProjectPIDPIC18F.c,335 :: 		PWM1_Start();
	CALL        _PWM1_Start+0, 0
;MyProjectPIDPIC18F.c,337 :: 		lcd_init();
	CALL        _Lcd_Init+0, 0
;MyProjectPIDPIC18F.c,338 :: 		lcd_cmd(_lcd_clear);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,339 :: 		lcd_cmd(_lcd_cursor_off);
	MOVLW       12
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,340 :: 		lcd_out(2,1,"Tocdo:");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr13_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr13_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,341 :: 		lcd_out(2,14,"M/P");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       14
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr14_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr14_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,342 :: 		lcd_out(1,7,"START");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       7
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr15_MyProjectPIDPIC18F+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr15_MyProjectPIDPIC18F+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,343 :: 		while(1)
L_main52:
;MyProjectPIDPIC18F.c,345 :: 		PWM1_Set_Duty(duty);
	MOVF        _duty+0, 0 
	MOVWF       R0 
	MOVF        _duty+1, 0 
	MOVWF       R1 
	MOVF        _duty+2, 0 
	MOVWF       R2 
	MOVF        _duty+3, 0 
	MOVWF       R3 
	CALL        _double2byte+0, 0
	MOVF        R0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;MyProjectPIDPIC18F.c,346 :: 		wordtostr(output,txt);
	MOVF        _output+0, 0 
	MOVWF       FARG_WordToStr_input+0 
	MOVF        _output+1, 0 
	MOVWF       FARG_WordToStr_input+1 
	MOVLW       _txt+0
	MOVWF       FARG_WordToStr_output+0 
	MOVLW       hi_addr(_txt+0)
	MOVWF       FARG_WordToStr_output+1 
	CALL        _WordToStr+0, 0
;MyProjectPIDPIC18F.c,348 :: 		if (coun>=1)
	MOVLW       128
	XORWF       _coun+1, 0 
	MOVWF       R0 
	MOVLW       128
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main95
	MOVLW       1
	SUBWF       _coun+0, 0 
L__main95:
	BTFSS       STATUS+0, 0 
	GOTO        L_main54
;MyProjectPIDPIC18F.c,349 :: 		{   UART1_Write_Text(txt);
	MOVLW       _txt+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(_txt+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;MyProjectPIDPIC18F.c,350 :: 		Delay_ms(10);
	MOVLW       65
	MOVWF       R12, 0
	MOVLW       238
	MOVWF       R13, 0
L_main55:
	DECFSZ      R13, 1, 1
	BRA         L_main55
	DECFSZ      R12, 1, 1
	BRA         L_main55
	NOP
;MyProjectPIDPIC18F.c,351 :: 		coun=0 ;
	CLRF        _coun+0 
	CLRF        _coun+1 
;MyProjectPIDPIC18F.c,352 :: 		}
L_main54:
;MyProjectPIDPIC18F.c,353 :: 		if (count>=10)
	MOVLW       128
	XORWF       _count+1, 0 
	MOVWF       R0 
	MOVLW       128
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main96
	MOVLW       10
	SUBWF       _count+0, 0 
L__main96:
	BTFSS       STATUS+0, 0 
	GOTO        L_main56
;MyProjectPIDPIC18F.c,355 :: 		lcd_out(2,8,txt);
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       8
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _txt+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_txt+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectPIDPIC18F.c,356 :: 		count=0 ;
	CLRF        _count+0 
	CLRF        _count+1 
;MyProjectPIDPIC18F.c,357 :: 		}
L_main56:
;MyProjectPIDPIC18F.c,359 :: 		if(dk==0)// su dung phim hex or may tinh
	MOVF        _dk+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main57
;MyProjectPIDPIC18F.c,360 :: 		quetphim();
	CALL        _quetphim+0, 0
L_main57:
;MyProjectPIDPIC18F.c,361 :: 		if( number!=0 && number!=10 && number!=11 && number!=12 && number!=13 && number!=14 && number!=15 && number!=16 && set==1)
	MOVF        _number+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       10
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       11
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       12
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       13
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       14
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       15
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _number+0, 0 
	XORLW       16
	BTFSC       STATUS+0, 2 
	GOTO        L_main60
	MOVF        _set+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main60
L__main79:
;MyProjectPIDPIC18F.c,363 :: 		mang[j]=number;
	MOVF        _j+0, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVLW       _mang+0
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       hi_addr(_mang+0)
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	MOVF        _number+0, 0 
	MOVWF       POSTINC1+0 
	MOVLW       0
	MOVWF       POSTINC1+0 
;MyProjectPIDPIC18F.c,365 :: 		j++;
	INCF        _j+0, 1 
;MyProjectPIDPIC18F.c,366 :: 		if(j>3)
	MOVF        _j+0, 0 
	SUBLW       3
	BTFSC       STATUS+0, 0 
	GOTO        L_main61
;MyProjectPIDPIC18F.c,367 :: 		j=3;//mang co toi da 3 phan tu
	MOVLW       3
	MOVWF       _j+0 
L_main61:
;MyProjectPIDPIC18F.c,368 :: 		setup();
	CALL        _setup+0, 0
;MyProjectPIDPIC18F.c,369 :: 		number=0;
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,370 :: 		}
L_main60:
;MyProjectPIDPIC18F.c,372 :: 		if(number==10)//--**--setup toc do--**--
	MOVF        _number+0, 0 
	XORLW       10
	BTFSS       STATUS+0, 2 
	GOTO        L_main62
;MyProjectPIDPIC18F.c,374 :: 		if(sta==1)//--**--nut start da duoc nhan--**--khi start thi sta=1--**--
	MOVF        _sta+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main63
;MyProjectPIDPIC18F.c,376 :: 		set=1;//--**--dk de biet setup da dc nhan--**--
	MOVLW       1
	MOVWF       _set+0 
;MyProjectPIDPIC18F.c,377 :: 		lcd_cmd(_lcd_clear);//--**--xoa man hinh truoc khi goi giao dien setup len--**--
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectPIDPIC18F.c,378 :: 		setup();
	CALL        _setup+0, 0
;MyProjectPIDPIC18F.c,380 :: 		}
L_main63:
;MyProjectPIDPIC18F.c,381 :: 		}
L_main62:
;MyProjectPIDPIC18F.c,382 :: 		if(number==11)//--**--backspace--**--
	MOVF        _number+0, 0 
	XORLW       11
	BTFSS       STATUS+0, 2 
	GOTO        L_main64
;MyProjectPIDPIC18F.c,384 :: 		if(set==1)//--**--khi setup da duoc nhan thi phim xoa moi thuc hien duoc--**--
	MOVF        _set+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main65
;MyProjectPIDPIC18F.c,386 :: 		j--;//--**--xoa nguoc mang--**--
	DECF        _j+0, 1 
;MyProjectPIDPIC18F.c,387 :: 		if(j<0)
	MOVLW       0
	SUBWF       _j+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_main66
;MyProjectPIDPIC18F.c,388 :: 		j=0;
	CLRF        _j+0 
L_main66:
;MyProjectPIDPIC18F.c,389 :: 		mang[j]=0;//--**--cho tung mang gia tri cua mang--**--
	MOVF        _j+0, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVLW       _mang+0
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       hi_addr(_mang+0)
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	CLRF        POSTINC1+0 
	CLRF        POSTINC1+0 
;MyProjectPIDPIC18F.c,391 :: 		setup();//--**--goi ra giao dien trinh setup--**--
	CALL        _setup+0, 0
;MyProjectPIDPIC18F.c,392 :: 		}
L_main65:
;MyProjectPIDPIC18F.c,393 :: 		}
L_main64:
;MyProjectPIDPIC18F.c,394 :: 		if(number==12)//--**--ok--**--
	MOVF        _number+0, 0 
	XORLW       12
	BTFSS       STATUS+0, 2 
	GOTO        L_main67
;MyProjectPIDPIC18F.c,396 :: 		if(set==1)//--**--setup phai duoc nhan truoc--**--
	MOVF        _set+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main68
;MyProjectPIDPIC18F.c,398 :: 		ok();
	CALL        _ok+0, 0
;MyProjectPIDPIC18F.c,401 :: 		set=0;
	CLRF        _set+0 
;MyProjectPIDPIC18F.c,402 :: 		}
L_main68:
;MyProjectPIDPIC18F.c,403 :: 		}
L_main67:
;MyProjectPIDPIC18F.c,404 :: 		if(number==13)//--**--cancel--**--
	MOVF        _number+0, 0 
	XORLW       13
	BTFSS       STATUS+0, 2 
	GOTO        L_main69
;MyProjectPIDPIC18F.c,406 :: 		if(set==1)//--**--setup da duoc nhan truoc do--**--
	MOVF        _set+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main70
;MyProjectPIDPIC18F.c,407 :: 		cancel();
	CALL        _cancel+0, 0
L_main70:
;MyProjectPIDPIC18F.c,408 :: 		set=0;//--**--reset phim setup--**--
	CLRF        _set+0 
;MyProjectPIDPIC18F.c,409 :: 		}
L_main69:
;MyProjectPIDPIC18F.c,410 :: 		if(number==14)//phim so 0
	MOVF        _number+0, 0 
	XORLW       14
	BTFSS       STATUS+0, 2 
	GOTO        L_main71
;MyProjectPIDPIC18F.c,412 :: 		if(set==1)//phim setup da duoc nhan
	MOVF        _set+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main72
;MyProjectPIDPIC18F.c,413 :: 		{ j++;
	INCF        _j+0, 1 
;MyProjectPIDPIC18F.c,414 :: 		if(j>3)
	MOVF        _j+0, 0 
	SUBLW       3
	BTFSC       STATUS+0, 0 
	GOTO        L_main73
;MyProjectPIDPIC18F.c,415 :: 		j=3;
	MOVLW       3
	MOVWF       _j+0 
L_main73:
;MyProjectPIDPIC18F.c,416 :: 		number=0;//cho number tro lai bang 0
	CLRF        _number+0 
;MyProjectPIDPIC18F.c,417 :: 		}
L_main72:
;MyProjectPIDPIC18F.c,418 :: 		}
L_main71:
;MyProjectPIDPIC18F.c,419 :: 		if(number==15)//--**--start--**--
	MOVF        _number+0, 0 
	XORLW       15
	BTFSS       STATUS+0, 2 
	GOTO        L_main74
;MyProjectPIDPIC18F.c,421 :: 		start();//--**--goi giao dien start ra--**--
	CALL        _start+0, 0
;MyProjectPIDPIC18F.c,422 :: 		sta=1;//--**--sta=1 la dieu kien de biet start da duoc nhan--**--
	MOVLW       1
	MOVWF       _sta+0 
;MyProjectPIDPIC18F.c,423 :: 		set=0;//--**--reset phim setup--**--
	CLRF        _set+0 
;MyProjectPIDPIC18F.c,424 :: 		}
L_main74:
;MyProjectPIDPIC18F.c,425 :: 		if(number==16)//--**--stop--**--
	MOVF        _number+0, 0 
	XORLW       16
	BTFSS       STATUS+0, 2 
	GOTO        L_main75
;MyProjectPIDPIC18F.c,427 :: 		stop();//--**--goi chuong trinh con stop ra--**--
	CALL        _stop+0, 0
;MyProjectPIDPIC18F.c,428 :: 		sta=0;//--**--reset bien dieu kien start--**--
	CLRF        _sta+0 
;MyProjectPIDPIC18F.c,429 :: 		set=0;//--**--reset bien dieu khien setup--**--
	CLRF        _set+0 
;MyProjectPIDPIC18F.c,430 :: 		for(k=0;k<j;k++)//--**--xoa mang da duoc nhap truoc do--**--
	CLRF        _k+0 
L_main76:
	MOVF        _j+0, 0 
	SUBWF       _k+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_main77
;MyProjectPIDPIC18F.c,431 :: 		mang[k]=0;
	MOVF        _k+0, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVLW       _mang+0
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       hi_addr(_mang+0)
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	CLRF        POSTINC1+0 
	CLRF        POSTINC1+0 
;MyProjectPIDPIC18F.c,430 :: 		for(k=0;k<j;k++)//--**--xoa mang da duoc nhap truoc do--**--
	INCF        _k+0, 1 
;MyProjectPIDPIC18F.c,431 :: 		mang[k]=0;
	GOTO        L_main76
L_main77:
;MyProjectPIDPIC18F.c,432 :: 		j=0;//--**--dua con tro ve vi tri dau mang--**--
	CLRF        _j+0 
;MyProjectPIDPIC18F.c,433 :: 		}
L_main75:
;MyProjectPIDPIC18F.c,434 :: 		}
	GOTO        L_main52
;MyProjectPIDPIC18F.c,436 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
