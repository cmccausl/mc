.function mc_main
  .param string arc_path
  .invoke parm_sort()
  .invoke rel_pseudoformalize()
  .//
  .// Create the unmarked, standard singletons.
  .invoke sys_singletons()
  .select any te_file from instances of TE_FILE
  .assign te_file.arc_path = arc_path
  .//
  .// marking
  .//
  .// Initialize the generator database with marking information.
  .// Note that the order of processing is important here.
  .//
  .// 2) Mark interrupt handlers.
  .include "${te_file.system_color_path}/${te_file.bridge_mark}"
  .// 3) Initiate user data type marking.
  .include "${te_file.system_color_path}/${te_file.datatype_mark}"
  .// 4) Initiate prefix marking (from system marking file).
  .include "${te_file.system_color_path}/${te_file.system_mark}"
  .//
  .invoke oal( "mark_pass("1");" )
  .invoke sys_populate()
  .invoke oal( "mark_pass("2");" )
  .select any te_sys from instances of TE_SYS
  .//
  .// 5) Perform domain level marking.
  .include "${te_file.domain_color_path}/${te_file.domain_mark}"
  .//
  .// 6) Perform class level marking.
  .include "${te_file.domain_color_path}/${te_file.class_mark}"
  .//
  .// 7) Perform event marking.
  .include "${te_file.domain_color_path}/${te_file.event_mark}"
  .//
  .// analyze
  .invoke sys_analyze( te_sys )
  .invoke CreateSpecialWhereClauseInstances( te_sys )
  .select many te_cs from instances of TE_C where ( selected.included_in_build )
  .for each te_c in te_cs
    .// Propagate domain information to the system level.
    .invoke te_c_CollectLimits( te_c )
    .select many te_classs related by te_c->TE_CLASS[R2064] where ( not selected.ExcludeFromGen )
    .invoke class_sort( te_classs )
  .end for
  .print "translating values/expressions"
  .invoke val_translate()
  .invoke oal_translate()
.end function
