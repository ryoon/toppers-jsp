STACKTOP    .set      0x4000    ; タスク独立部用スタックの初期値 
TCB_sp      .set      6
TCB_pc      .set      7
            .ref      cinit,___bss__,___end__
            .ref      _start,_interrupt
            .ref      _nest
            .ref      _INT_VECNO
            .ref      __kernel_reqflg
            .ref      __kernel_enadsp
            .ref      __kernel_runtsk,__kernel_schedtsk
            .ref      _int_table
            .ref      _int_mask_table
            .ref      _hardware_init_hook,_software_init_hook
            .ref      __kernel_calltex
            .ref      _kernel_start

