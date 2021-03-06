T_b("/*---------------------------------------------------------------------");
T_b("\n");
T_b(" ");
T_b("* File:  ");
T_b(te_file->persist);
T_b(".");
T_b(te_file->src_file_ext);
T_b("\n");
T_b(" ");
T_b("*");
T_b("\n");
T_b(" ");
T_b("* Description:");
T_b("\n");
T_b(" ");
T_b("* This file provides persistence mechanisms to maintain instances and");
T_b("\n");
T_b(" ");
T_b("* associations across power and reset cycles.");
T_b("\n");
T_b(" ");
T_b("*");
T_b("\n");
T_b(" ");
T_b("* ");
T_b(te_copyright->body);
T_b("\n");
T_b(" ");
T_b("*-------------------------------------------------------------------*/");
T_b("\n");
T_b("#include \"");
T_b(te_file->types);
T_b(".");
T_b(te_file->hdr_file_ext);
T_b("\"");
T_b("\n");
T_b("#include \"");
T_b(te_file->nvs_bridge);
T_b(".");
T_b(te_file->hdr_file_ext);
T_b("\"");
T_b("\n");
T_b("#include \"");
T_b(te_file->persist);
T_b(".");
T_b(te_file->hdr_file_ext);
T_b("\"");
T_b("\n");
T_b(persist_class_union);
T_b("\n");
T_b("#define PERSIST_LARGEST_CLASS sizeof( ");
T_b(persist_class_union->uname);
T_b(" )");
T_b("\n");
T_b("#define PERSIST_LINK_TYPE 0");
T_b("\n");
T_b("#define PERSIST_INSTANCE_TYPE 1");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* operation - 8 bits:");
T_b("\n");
T_b(" ");
T_b("*   Top nibble is operation.  Bottom nibble is item type.");
T_b("\n");
T_b(" ");
T_b("*   MSB  0 = delete, 1 = update");
T_b("\n");
T_b(" ");
T_b("*        0 =         1");
T_b("\n");
T_b(" ");
T_b("*        0 =         1");
T_b("\n");
T_b(" ");
T_b("*        0 =         1");
T_b("\n");
T_b(" ");
T_b("*        0 =         1");
T_b("\n");
T_b(" ");
T_b("*        0 =         1");
T_b("\n");
T_b(" ");
T_b("*        0 =         1");
T_b("\n");
T_b(" ");
T_b("*   LSB  0 = link,   1 = instance");
T_b("\n");
T_b(" ");
T_b("*");
T_b("\n");
T_b(" ");
T_b("*   00 = delete link");
T_b("\n");
T_b(" ");
T_b("*   01 = delete instance");
T_b("\n");
T_b(" ");
T_b("*   80 = update link");
T_b("\n");
T_b(" ");
T_b("*   81 = update instance");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* We need access to the ");
T_b(te_cia->class_info_name);
T_b(" to access the instance");
T_b("\n");
T_b(" ");
T_b("* management structure.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("extern ");
T_b(te_cia->class_info_type);
T_b(" * const * const ");
T_b(te_cia->class_info_name);
T_b("[];");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* insts and links are dynamic lists of dirty instances and links");
T_b("\n");
T_b(" ");
T_b("* that need to be flushed to the non-volatile storage (upon on a");
T_b("\n");
T_b(" ");
T_b("* commit operation).");
T_b("\n");
T_b(" ");
T_b("* instlinks_active and insts/link_inactive are the collection of");
T_b("\n");
T_b(" ");
T_b("* animate and inanimate persistent instances and links.");
T_b("\n");
T_b(" ");
T_b("* inst/link_container is the list of containoids used to manage the");
T_b("\n");
T_b(" ");
T_b("* collection of persistent instances/links.");
T_b("\n");
T_b(" ");
T_b("* inst/link_pool represents the actual storage for these managed");
T_b("\n");
T_b(" ");
T_b("* instances/links.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("typedef struct {");
T_b("\n");
T_b("  ");
T_b(te_persist->instid_type);
T_b(" ");
T_b(te_persist->instid_name);
T_b(";");
T_b("\n");
T_b("  ");
T_b(te_instance->handle);
T_b(" instance;");
T_b("\n");
T_b("} persist_instance_t;");
T_b("\n");
T_b("typedef struct { u1_t operation; persist_instance_t inst; } op_inst_t;");
T_b("\n");
T_b("typedef struct { u1_t operation; ");
T_b(link_type_name);
T_b(" link; } op_link_t;");
T_b("\n");
T_b("typedef union { op_inst_t inst; op_link_t link; } instlink_t;");
T_b("\n");
T_b("static ");
T_b(te_set->scope);
T_b(te_set->base_class);
T_b(" insts_inactive, links_inactive;");
T_b("\n");
T_b("static struct { Escher_SetElement_s * head, * tail; } instlinks_active;");
T_b("\n");
T_b("static ");
T_b(te_set->scope);
T_b(te_set->element_type);
T_b(" inst_container[ ");
T_b(te_persist->instance_cache_depth);
T_b(" ];");
T_b("\n");
T_b("static ");
T_b(te_set->scope);
T_b(te_set->element_type);
T_b(" link_container[ ");
T_b(te_persist->link_cache_depth);
T_b(" ];");
T_b("\n");
T_b("static op_inst_t inst_pool[ ");
T_b(te_persist->instance_cache_depth);
T_b(" ] ;");
T_b("\n");
T_b("static op_link_t link_pool[ ");
T_b(te_persist->link_cache_depth);
T_b(" ] ;");
T_b("\n");
T_b("static bool restoring = false;   /* flag to prevent recursion */");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Return true if input class is active (having a state machine)->");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("#ifdef MC3020_DEFAULT_STATE");
T_b("\n");
T_b("static bool is_active_class( const ");
T_b(te_typemap->domain_number_name);
T_b(", const ");
T_b(te_typemap->object_number_name);
T_b(" );");
T_b("\n");
T_b("static bool is_active_class(");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(" ");
T_b(domain_num_var);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" class_num");
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("bool rc = false;");
T_b("\n");
T_b("  ");
T_b("static const ");
T_b(te_typemap->object_number_name);
T_b(" ");
T_b(te_cia->active_count);
T_b("[ SYSTEM_DOMAIN_COUNT ] = {");
T_b("\n");
T_b(active_class_counts);
T_b("");
T_b("  ");
T_b("};");
T_b("\n");
T_b("  ");
T_b("if ( class_num < ");
T_b(te_cia->active_count);
T_b("[ ");
T_b(domain_num_var);
T_b(" ] ) {");
T_b("\n");
T_b("    ");
T_b("rc = true;");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("return rc;");
T_b("\n");
T_b("}");
T_b("\n");
T_b("#endif");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Given the instance handle and class number, return the instance index");
T_b("\n");
T_b(" ");
T_b("* (into the instance collection).");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("static ");
T_b(te_typemap->instance_index_name);
T_b(" ");
T_b(te_prefix->result);
T_b("getindex(");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_instance->handle);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" );");
T_b("\n");
T_b("static ");
T_b(te_typemap->instance_index_name);
T_b(" ");
T_b(te_prefix->result);
T_b("getindex( ");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_instance->handle);
T_b(" instance,");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(" ");
T_b(domain_num_var);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" class_num");
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = *( ");
T_b(te_cia->class_info_name);
T_b("[ ");
T_b(domain_num_var);
T_b(" ] + class_num );");
T_b("\n");
T_b("  ");
T_b("return ( ((c_t *) instance - (c_t *) dci->pool ) / dci->size );");
T_b("\n");
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Given the class number and the instance index, return the corresponding");
T_b("\n");
T_b(" ");
T_b("* instance handle.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("static ");
T_b(te_instance->handle);
T_b(" ");
T_b(te_prefix->result);
T_b("getinstance(");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->instance_index_name);
T_b(" );");
T_b("\n");
T_b("static ");
T_b(te_instance->handle);
T_b(" ");
T_b(te_prefix->result);
T_b("getinstance( ");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(" ");
T_b(domain_num_var);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" class_num,");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->instance_index_name);
T_b(" index");
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = *( ");
T_b(te_cia->class_info_name);
T_b("[ ");
T_b(domain_num_var);
T_b(" ] + class_num );");
T_b("\n");
T_b("  ");
T_b("return (");
T_b(te_instance->handle);
T_b(")");
T_b("\n");
T_b("    ");
T_b("( (c_t *) dci->pool + ( dci->size * index ) );");
T_b("\n");
T_b("}");
T_b("\n");
if ( te_thread->enabled ) {
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("extern bool ");
T_b(te_eq->run_flag);
T_b(";");
T_b("\n");
if ( te_thread->flavor == "Nucleus" ) {
T_b("static void persist_commit_loop( void * );");
T_b("\n");
T_b("static void persist_commit_loop( void * thread )");
T_b("\n");
} else {
T_b("static void * persist_commit_loop( void * );");
T_b("\n");
T_b("static void * persist_commit_loop( void * thread )");
T_b("\n");
}
T_b("{");
T_b("\n");
T_b("  ");
T_b("u1_t t = *( (u1_t *) thread );");
T_b("\n");
T_b("  ");
T_b("while ( true == ");
T_b(te_eq->run_flag);
T_b(" ) {");
T_b("\n");
T_b("    ");
T_b(te_persist->commit);
T_b("();");
T_b("\n");
T_b("    ");
T_b(te_thread->nonbusy_wait);
T_b("( t );");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b(te_persist->commit);
T_b("();");
T_b("\n");
if ( te_thread->flavor != "Nucleus" ) {
T_b("  ");
T_b("return 0;");
T_b("\n");
}
T_b("}");
T_b("\n");
T_b("#endif");
T_b("\n");
}
T_b("static void Escher_PostPersistentInstance(");
T_b("\n");
T_b(persist_check_mark->arglist_types);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const u1_t );");
T_b("\n");
T_b("static void Escher_PostPersistentInstance(");
T_b("\n");
T_b(persist_check_mark->arglist);
T_b(",");
T_b("\n");
T_b("  ");
T_b("const u1_t operation");
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("op_inst_t * opinst;");
T_b("\n");
T_b("  ");
T_b(te_set->scope);
T_b(te_set->element_type);
T_b(" * node;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("  ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("  ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("  ");
T_b("#endif");
T_b("\n");
} else {
T_b("  ");
T_b("if ( insts_inactive->head == 0 ) { /* Instance cache is full->  */");
T_b("\n");
T_b("    ");
T_b(te_persist->commit);
T_b("(); /* Flush to NVS.  */");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
}
T_b("  ");
T_b("node = insts_inactive.head; /* Shift item from inactive to active.  */");
T_b("\n");
T_b("  ");
T_b("insts_inactive.head = insts_inactive.head->next;");
T_b("\n");
T_b("  ");
T_b("opinst = (op_inst_t *) node->object;");
T_b("\n");
T_b("  ");
T_b("/* Post to FIFO queue.  */");
T_b("\n");
T_b("  ");
T_b("node->next = 0;");
T_b("\n");
T_b("  ");
T_b("if ( instlinks_active->tail == 0 ) {");
T_b("\n");
T_b("    ");
T_b("instlinks_active.head = node;");
T_b("\n");
T_b("  ");
T_b("} else {");
T_b("\n");
T_b("    ");
T_b("instlinks_active.tail->next = node;");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("instlinks_active.tail = node;");
T_b("\n");
T_b("  ");
T_b("opinst->operation = operation;");
T_b("\n");
T_b("  ");
T_b("opinst->inst.instance_identifier.domainnum = domain_num;");
T_b("\n");
T_b("  ");
T_b("opinst->inst.instance_identifier.classnum = class_num;");
T_b("\n");
T_b("  ");
T_b("opinst->inst.instance_identifier.index =");
T_b("\n");
T_b("    ");
T_b(te_prefix->result);
T_b("getindex( instance, ");
T_b(domain_num_var);
T_b(", class_num );");
T_b("\n");
T_b("  ");
T_b("opinst->inst.instance = instance;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("  ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("  ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("  ");
T_b(te_thread->nonbusy_wake);
T_b("( PERSISTENCE_THREAD_NUMBER );");
T_b("\n");
T_b("  ");
T_b("#endif");
T_b("\n");
}
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Remove the instance from the non-volatile store.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void");
T_b("\n");
T_b(te_persist->remove);
T_b("(");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_instance->handle);
T_b(" instance,");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(" domain_num,");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" class_num");
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("Escher_PostPersistentInstance( instance, domain_num, class_num, 0x01 );");
T_b("\n");
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Initialize the system level collection structures (for links).");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void");
T_b("\n");
T_b(te_persist->factory_init);
T_b("( void )");
T_b("\n");
T_b("{");
T_b("\n");
if ( te_thread->enabled ) {
T_b("  ");
T_b("u1_t thread = PERSISTENCE_THREAD_NUMBER;");
T_b("\n");
}
T_b("  ");
T_b("links_inactive.head = ");
T_b(te_prefix->result);
T_b("SetInsertBlock(");
T_b("\n");
T_b("    ");
T_b("&link_container[0], (const u1_t *) &link_pool[0],");
T_b("\n");
T_b("    ");
T_b("sizeof( op_link_t ), ");
T_b(te_persist->link_cache_depth);
T_b(" );");
T_b("\n");
T_b("  ");
T_b("insts_inactive.head = ");
T_b(te_prefix->result);
T_b("SetInsertBlock(");
T_b("\n");
T_b("    ");
T_b("&inst_container[0], (const u1_t *) &inst_pool[0],");
T_b("\n");
T_b("    ");
T_b("sizeof( op_inst_t ), ");
T_b(te_persist->instance_cache_depth);
T_b(" );");
T_b("\n");
if ( te_thread->enabled ) {
T_b("  ");
T_b(te_thread->create);
T_b("( persist_commit_loop, thread );");
T_b("\n");
}
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Check, mark and post the given instance to the persistent flush queue.");
T_b("\n");
T_b(" ");
T_b("* This routine places instances into the queue only if they have not");
T_b("\n");
T_b(" ");
T_b("* already been posted and marked as dirty.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b(persist_check_mark->type);
T_b("\n");
T_b(persist_check_mark->name);
T_b("(");
T_b("\n");
T_b(persist_check_mark->arglist);
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("/* Check if instance is \"clean\"->  If so (first mod), then queue it->  */");
T_b("\n");
T_b("  ");
T_b("if ( instance->");
T_b(te_persist->dirty_name);
T_b(" == ");
T_b(te_persist->dirty_clean);
T_b(" ) {");
T_b("\n");
T_b("    ");
T_b("instance->");
T_b(te_persist->dirty_name);
T_b(" = ");
T_b(te_persist->dirty_dirty);
T_b("; /* Mark dirty.  */");
T_b("\n");
T_b("    ");
T_b("Escher_PostPersistentInstance( instance, domain_num, class_num, 0x81 );");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Queue a link to be persisted.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void ");
T_b(persist_post_link->name);
T_b("(");
T_b("\n");
T_b(persist_post_link->arglist);
T_b("\n");
T_b(")");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("if ( restoring != true ) {");
T_b("\n");
T_b("    ");
T_b("op_link_t * oplink;");
T_b("\n");
T_b("    ");
T_b(te_set->scope);
T_b(te_set->element_type);
T_b(" * node;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("    ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("    ");
T_b("#endif");
T_b("\n");
} else {
T_b("    ");
T_b("if ( links_inactive->head == 0 ) { /* Link cache is full->  */");
T_b("\n");
T_b("      ");
T_b(te_persist->commit);
T_b("(); /* Flush to NVS.  */");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
}
T_b("    ");
T_b("node = links_inactive.head; /* Shift item from inactive to active.  */");
T_b("\n");
T_b("    ");
T_b("links_inactive.head = links_inactive.head->next;");
T_b("\n");
T_b("    ");
T_b("/* Post to FIFO queue.  */");
T_b("\n");
T_b("    ");
T_b("node->next = 0;");
T_b("\n");
T_b("    ");
T_b("if ( instlinks_active->tail == 0 ) {");
T_b("\n");
T_b("      ");
T_b("instlinks_active.head = node;");
T_b("\n");
T_b("    ");
T_b("} else {");
T_b("\n");
T_b("      ");
T_b("instlinks_active.tail->next = node;");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
T_b("    ");
T_b("instlinks_active.tail = node;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("    ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("    ");
T_b("#endif");
T_b("\n");
}
T_b("    ");
T_b("oplink = (op_link_t *) node->object;");
T_b("\n");
T_b("    ");
T_b("/* Load with input data.  */");
T_b("\n");
T_b("    ");
T_b("oplink->link.owner.domainnum = domain;");
T_b("\n");
T_b("    ");
T_b("oplink->link.owner.classnum = owning_class;");
T_b("\n");
T_b("    ");
T_b("oplink->link.owner.index = ri;");
T_b("\n");
T_b("    ");
T_b("oplink->link.left.domainnum = domain;");
T_b("\n");
T_b("    ");
T_b("oplink->link.left.classnum = l;");
T_b("\n");
T_b("    ");
T_b("oplink->link.left.index = ");
T_b(te_prefix->result);
T_b("getindex( left, domain, l );");
T_b("\n");
T_b("    ");
T_b("oplink->link.right.domainnum = domain;");
T_b("\n");
T_b("    ");
T_b("oplink->link.right.classnum = r;");
T_b("\n");
T_b("    ");
T_b("oplink->link.right.index = ");
T_b(te_prefix->result);
T_b("getindex( right, domain, r );");
T_b("\n");
T_b("    ");
T_b("if ( assoc != 0 ) { /* associative class present */");
T_b("\n");
T_b("      ");
T_b("oplink->link.assoc.domainnum = domain;");
T_b("\n");
T_b("      ");
T_b("oplink->link.assoc.classnum = a;");
T_b("\n");
T_b("      ");
T_b("oplink->link.assoc.index = ");
T_b(te_prefix->result);
T_b("getindex( assoc, domain, a );");
T_b("\n");
T_b("    ");
T_b("} else {");
T_b("\n");
T_b("      ");
T_b("oplink->link.assoc.domainnum = 0;");
T_b("\n");
T_b("      ");
T_b("oplink->link.assoc.classnum = 0;");
T_b("\n");
T_b("      ");
T_b("oplink->link.assoc.index = 0;");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
T_b("    ");
T_b("if ( operation == 0 ) {");
T_b("\n");
T_b("      ");
T_b("oplink->operation = 0x80; /* insert/update + link */");
T_b("\n");
T_b("    ");
T_b("} else if ( operation == 1 ) {");
T_b("\n");
T_b("      ");
T_b("oplink->operation = 0x00; /* delete + link */");
T_b("\n");
T_b("    ");
T_b("} else {");
T_b("\n");
T_b("      ");
T_b(te_callout->persistence_error);
T_b("( 0x44 );");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("    ");
T_b(te_thread->nonbusy_wake);
T_b("( PERSISTENCE_THREAD_NUMBER );");
T_b("\n");
T_b("    ");
T_b("#endif");
T_b("\n");
}
T_b("  ");
T_b("} /* not restoring */");
T_b("\n");
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Commit all queued dirty instances and links to non-volatile");
T_b("\n");
T_b(" ");
T_b("* storage.  Iterate through the accumulated list bridging to the");
T_b("\n");
T_b(" ");
T_b("* NVS domain to write or update the elements.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("i_t");
T_b("\n");
T_b(te_persist->commit);
T_b("( void )");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("i_t rc = 0;");
T_b("\n");
T_b("  ");
T_b("u1_t work_to_do;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("  ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("  ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("  ");
T_b("#endif");
T_b("\n");
}
T_b("  ");
T_b("work_to_do = ( instlinks_active.head != 0 ) ? 1 : 0;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("  ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("  ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("  ");
T_b("#endif");
T_b("\n");
}
T_b("  ");
T_b("while ( work_to_do != 0 ) {");
T_b("\n");
T_b("    ");
T_b(te_set->scope);
T_b(te_set->element_type);
T_b(" * node;");
T_b("\n");
T_b("    ");
T_b("instlink_t * ili;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("    ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("    ");
T_b("#endif");
T_b("\n");
}
T_b("    ");
T_b("node = instlinks_active.head; /* Shift item off of active queue.  */");
T_b("\n");
T_b("    ");
T_b("instlinks_active.head = instlinks_active.head->next;");
T_b("\n");
T_b("    ");
T_b("if ( instlinks_active->head == 0 ) {");
T_b("\n");
T_b("      ");
T_b("instlinks_active.tail = 0;");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
T_b("    ");
T_b("ili = (instlink_t *) node->object;");
T_b("\n");
T_b("    ");
T_b("if ( ( ili->inst->operation & 0x0f ) == 1 ) {");
T_b("\n");
T_b("      ");
T_b("/* Item is instance.  */");
T_b("\n");
T_b("      ");
T_b(te_typemap->domain_number_name);
T_b(" domain_num;");
T_b("\n");
T_b("      ");
T_b(te_typemap->object_number_name);
T_b(" class_num;");
T_b("\n");
T_b("      ");
T_b(te_typemap->instance_index_name);
T_b(" index_num;");
T_b("\n");
T_b("      ");
T_b(te_cia->class_info_type);
T_b(" * dci;");
T_b("\n");
T_b("      ");
T_b("node->next = insts_inactive.head; /* Shift item onto inst queue.  */");
T_b("\n");
T_b("      ");
T_b("insts_inactive.head = node;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("      ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("      ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("      ");
T_b("#endif");
T_b("\n");
}
T_b("      ");
T_b("domain_num = ili->inst.inst.instance_identifier.domainnum;");
T_b("\n");
T_b("      ");
T_b("class_num = ili->inst.inst.instance_identifier.classnum;");
T_b("\n");
T_b("      ");
T_b("index_num = ili->inst.inst.instance_identifier.index;");
T_b("\n");
T_b("      ");
T_b("dci = *( ");
T_b(te_cia->class_info_name);
T_b("[ domain_num ] + class_num );");
T_b("\n");
T_b("      ");
T_b("if ( ili->inst->operation == 0x81 ) {");
T_b("\n");
T_b("        ");
T_b("if ( ( rc = NVS_insert( (domain_num << 24) + (class_num << 16) +");
T_b("\n");
T_b("          ");
T_b("ili->inst.inst.instance_identifier.index, dci->size_no_rel,");
T_b("\n");
T_b("          ");
T_b("( c_t const * ) ili->inst.inst.instance, PERSIST_INSTANCE_TYPE )");
T_b("\n");
T_b("           ");
T_b(") >= 0 ) {");
T_b("\n");
T_b("          ");
T_b("ili->inst.inst.instance->");
T_b(te_persist->dirty_name);
T_b(" = ");
T_b(te_persist->dirty_clean);
T_b(";");
T_b("\n");
T_b("          ");
T_b("rc = 0;");
T_b("\n");
T_b("        ");
T_b("} else {");
T_b("\n");
T_b("          ");
T_b(te_callout->persistence_error);
T_b("( rc );");
T_b("\n");
T_b("          ");
T_b("break;");
T_b("\n");
T_b("        ");
T_b("}");
T_b("\n");
T_b("      ");
T_b("} else if ( ili->inst->operation == 0x01 ) {");
T_b("\n");
T_b("        ");
T_b("NVS_remove( (domain_num << 24) + (class_num << 16) + index_num,");
T_b("\n");
T_b("          ");
T_b("0, 0, PERSIST_INSTANCE_TYPE );");
T_b("\n");
T_b("      ");
T_b("} else {");
T_b("\n");
T_b("        ");
T_b(te_callout->persistence_error);
T_b("( 0x77 );");
T_b("\n");
T_b("        ");
T_b("break;");
T_b("\n");
T_b("      ");
T_b("}");
T_b("\n");
T_b("    ");
T_b("} else if ( ( ili->link->operation & 0x0f ) == 0 ) {");
T_b("\n");
T_b("      ");
T_b("/* Item is link.  */");
T_b("\n");
T_b("      ");
T_b("node->next = links_inactive.head; /* Shift item onto link queue.  */");
T_b("\n");
T_b("      ");
T_b("links_inactive.head = node;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("      ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("      ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("      ");
T_b("#endif");
T_b("\n");
}
T_b("      ");
T_b("if ( ili->link->operation == 0x80 ) {");
T_b("\n");
T_b("        ");
T_b("if ( ( rc = NVS_insert( 0, sizeof( ");
T_b(link_type_name);
T_b(" ),");
T_b("\n");
T_b("          ");
T_b("( c_t const * ) &ili->link.link, PERSIST_LINK_TYPE ) ) >= 0 ) {");
T_b("\n");
T_b("          ");
T_b("rc = 0;");
T_b("\n");
T_b("        ");
T_b("} else {");
T_b("\n");
T_b("          ");
T_b(te_callout->persistence_error);
T_b("( rc );");
T_b("\n");
T_b("          ");
T_b("break;");
T_b("\n");
T_b("        ");
T_b("}");
T_b("\n");
T_b("      ");
T_b("} else if ( ili->link->operation == 0x00 ) {");
T_b("\n");
T_b("        ");
T_b("NVS_remove( 0, sizeof( ");
T_b(link_type_name);
T_b(" ),");
T_b("\n");
T_b("          ");
T_b("( c_t const * ) &ili->link.link, PERSIST_LINK_TYPE );");
T_b("\n");
T_b("      ");
T_b("} else {");
T_b("\n");
T_b("        ");
T_b(te_callout->persistence_error);
T_b("( 0x88 );");
T_b("\n");
T_b("        ");
T_b("break;");
T_b("\n");
T_b("      ");
T_b("}");
T_b("\n");
T_b("    ");
T_b("} else {");
T_b("\n");
T_b("      ");
T_b(te_callout->persistence_error);
T_b("( 0x99 );");
T_b("\n");
T_b("      ");
T_b("break;");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("    ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("    ");
T_b("#endif");
T_b("\n");
}
T_b("    ");
T_b("work_to_do = ( instlinks_active.head != 0 ) ? 1 : 0;");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b("#ifdef ");
T_b(te_prefix->define_u);
T_b("TASKING_");
T_b(te_thread->flavor);
T_b("\n");
T_b("    ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_PERSIST );");
T_b("\n");
T_b("    ");
T_b("#endif");
T_b("\n");
}
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("return rc;");
T_b("\n");
T_b("}");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Restore instances and links from non-volatile storage (NVS).");
T_b("\n");
T_b(" ");
T_b("* First, restore the instances.");
T_b("\n");
T_b(" ");
T_b("* Then, restore the links by calling the Link routines from the");
T_b("\n");
T_b(" ");
T_b("* various owning classes.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("i_t");
T_b("\n");
T_b(te_persist->restore);
T_b("( void )");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b("i_t rc = 0, type;");
T_b("\n");
T_b("  ");
T_b("c_t buffer[ PERSIST_LARGEST_CLASS + 1 ];");
T_b("\n");
T_b("  ");
T_b("c_t done = 0;");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci;");
T_b("\n");
T_b("  ");
T_b("u4_t key;");
T_b("\n");
T_b("  ");
T_b("i_t length;");
T_b("\n");
T_b("  ");
T_b("restoring = true;");
T_b("\n");
T_b("  ");
T_b("NVS_initialize();");
T_b("\n");
T_b("  ");
T_b("/* Restore instances.  */");
T_b("\n");
T_b("  ");
T_b("while ( done == 0 ) {");
T_b("\n");
T_b("    ");
T_b(te_typemap->domain_number_name);
T_b(" dn;");
T_b("\n");
T_b("    ");
T_b(te_typemap->object_number_name);
T_b(" cn;");
T_b("\n");
T_b("    ");
T_b(te_typemap->instance_index_name);
T_b(" ii;");
T_b("\n");
T_b("    ");
T_b(te_set->scope);
T_b(te_set->element_type);
T_b(" * node;");
T_b("\n");
T_b("    ");
T_b("length = NVS_next( (i_t *) &key, PERSIST_LARGEST_CLASS, &buffer[0], &type );");
T_b("\n");
T_b("    ");
T_b("if ( length <= 0 ) {");
T_b("\n");
T_b("      ");
T_b("done = 1;");
T_b("\n");
T_b("    ");
T_b("} else if ( type == PERSIST_INSTANCE_TYPE ) {");
T_b("\n");
T_b("      ");
T_b(te_instance->handle);
T_b(" i1, i2;");
T_b("\n");
T_b("      ");
T_b("i1 = (");
T_b(te_instance->handle);
T_b(") buffer;");
T_b("\n");
T_b("      ");
T_b("ii = (");
T_b(te_typemap->instance_index_name);
T_b(") ( key & 0x0000ffff );");
T_b("\n");
T_b("      ");
T_b("cn = (");
T_b(te_typemap->object_number_name);
T_b(") ( ( key >> 16 ) & 0x000000ff );");
T_b("\n");
T_b("      ");
T_b("dn = (");
T_b(te_typemap->domain_number_name);
T_b(") ( ( key >> 24 ) & 0x000000ff );");
T_b("\n");
T_b("      ");
T_b("dci = *( ");
T_b(te_cia->class_info_name);
T_b("[ dn ] + cn );");
T_b("\n");
T_b("      ");
T_b("i1->");
T_b(te_persist->dirty_name);
T_b(" = ");
T_b(te_persist->dirty_clean);
T_b("; /* Mark as \"clean\".  */");
T_b("\n");
if ( te_sys->CollectionsFlavo {
T_b("      ");
T_b("node = dci->container + ii; /* pointer arithmetic */");
T_b("\n");
T_b("      ");
T_b(te_dlist->remove_node);
T_b("( &dci->inactive, node );");
T_b("\n");
} else {
T_b("      ");
T_b("node = ");
T_b(te_slist->remove_node);
T_b("( &dci->inactive,");
T_b("\n");
T_b("        ");
T_b("(c_t *) dci->pool + ( ii * dci->size ) );");
T_b("\n");
}
T_b("      ");
T_b("i2 = (");
T_b(te_instance->handle);
T_b(") node->object;");
T_b("\n");
T_b("      ");
T_b(te_set->insert_instance);
T_b("( &dci->active, node );");
T_b("\n");
T_b("      ");
T_b(te_string->memmove);
T_b("( i2, buffer, dci->size_no_rel );");
T_b("\n");
T_b("      ");
T_b("#ifdef MC3020_DEFAULT_STATE");
T_b("\n");
T_b("      ");
T_b("if ( is_active_class( dn, cn ) ) {");
T_b("\n");
T_b("        ");
T_b("i2->");
T_b(te_instance->current_state);
T_b(" = dci->");
T_b(te_extent->istate_name);
T_b(";");
T_b("\n");
T_b("      ");
T_b("}");
T_b("\n");
T_b("      ");
T_b("#endif");
T_b("\n");
T_b("    ");
T_b("} else { /* MISRA nop */ }");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("done = 0;");
T_b("\n");
T_b("  ");
T_b("NVS_initialize();");
T_b("\n");
T_b("  ");
T_b("/* Restore links.  */");
T_b("\n");
T_b("  ");
T_b("while ( done == 0 ) {");
T_b("\n");
T_b("    ");
T_b("length = NVS_next( (i_t *) &key, PERSIST_LARGEST_CLASS, &buffer[0], &type );");
T_b("\n");
T_b("    ");
T_b("if ( length <= 0 ) {");
T_b("\n");
T_b("      ");
T_b("done = 1;");
T_b("\n");
T_b("    ");
T_b("} else if ( type == PERSIST_LINK_TYPE ) {");
T_b("\n");
T_b("      ");
T_b("/* Populate local link structure with info from the NVS buffer.");
T_b("\n");
T_b("       ");
T_b("* Call the appropriate link routine.");
T_b("\n");
T_b("       ");
T_b("*/");
T_b("\n");
T_b("      ");
T_b(link_type_name);
T_b(" link;");
T_b("\n");
T_b("      ");
T_b(te_string->memmove);
T_b("( &link, buffer, sizeof( ");
T_b(link_type_name);
T_b(" ) );");
T_b("\n");
T_b("      ");
T_b("dci = *( ");
T_b(te_cia->class_info_name);
T_b("[ link.owner.domainnum ]");
T_b("\n");
T_b("        ");
T_b("+ link.owner.classnum );");
T_b("\n");
T_b("      ");
T_b("dci->link_function( link.owner.index,");
T_b("\n");
T_b("        ");
T_b(te_prefix->result);
T_b("getinstance(");
T_b("\n");
T_b("          ");
T_b("link.left.domainnum, link.left.classnum, link.left.index ),");
T_b("\n");
T_b("        ");
T_b(te_prefix->result);
T_b("getinstance(");
T_b("\n");
T_b("          ");
T_b("link.right.domainnum, link.right.classnum, link.right.index ),");
T_b("\n");
T_b("        ");
T_b(te_prefix->result);
T_b("getinstance(");
T_b("\n");
T_b("          ");
T_b("link.assoc.domainnum, link.assoc.classnum, link.assoc.index )");
T_b("\n");
T_b("      ");
T_b(");");
T_b("\n");
T_b("    ");
T_b("} else { /* MISRA nop */ }");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("restoring = false;");
T_b("\n");
T_b("  ");
T_b("return rc;");
T_b("\n");
T_b("}");
T_b("\n");
