.. -*- rst -*-
 c4mk
======

A simple Makefile project all in c language.

License: BSD-3-Clause.
Copyright (c) 2026 player <playerc@msn.cn>

Makefile.c Example
------------------

::

   void make_rule(const struct c4mk_op *mk){
     mk->name("project name");
     mk->version("v0.0.1");
     
     mk->add_flag("ccflag: -a -b -c....");/* if have */
     mk->add_flag("ldflag: -a -b -c");

     mk->add_handler();
     
     mk->trg_begin("exe: cvd");
     mk->add_depend("c: a.c"");
     mk->add_depend("liba: liba");
     mk->add_depend("libso: libasdfs");
     mk->trg_end();

     mk->trg_begin("liba: libgod");
     mk->trg_end();

     mk->trg_begin("target: copyfile");
     mk->trg_end();
   }

:~ EOF
