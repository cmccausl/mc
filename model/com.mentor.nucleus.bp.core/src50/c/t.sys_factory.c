T_b("/*--------------------------------------------------------------------------");
T_b("\n");
T_b(" ");
T_b("* File:  ");
T_b(te_file->factory);
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
T_b("* ");
T_b(te_copyright->body);
T_b("\n");
T_b(" ");
T_b("*--------------------------------------------------------------------------*/");
T_b("\n");
T_b("#include \"");
T_b(te_file->types);
T_b(".");
T_b(te_file->hdr_file_ext);
T_b("\"");
T_b("\n");
if ( 0==strcmp("C",te_target->language) ) {
T_b(all_domain_include_files);
T_b("\n");
}
T_b(te_file->arc_path);
if ( te_sys->InstanceLoading ) {
T_b("Escher_iHandle_t Escher_instance_cache[ 1000000 ];");
T_b("\n");
}
if ( 0==strcmp("C",te_target->language) ) {
T_b(system_class_array->class_info);
T_b("\n");
}
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Create an instance of the class numbered on the input.");
T_b("\n");
T_b(" ");
T_b("* Return the handle of the created instance.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b(te_instance->handle);
T_b("\n");
T_b(te_instance->scope);
T_b(te_instance->create);
T_b("(");
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
T_b(te_set->element_type);
T_b(" * node;");
T_b("\n");
T_b("  ");
T_b(te_instance->handle);
T_b(" instance;");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = ");
if ( 0==strcmp("C++",te_target->language) ) {
T_b(te_instance->get_dci);
T_b("(class_num);");
T_b("\n");
} else {
T_b("*(");
T_b(te_cia->class_info_name);
T_b("[ ");
T_b(domain_num_var);
T_b(" ] + class_num);");
T_b("\n");
}
if ( te_thread->enabled ) {
T_b("  ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_INSTANCE );");
T_b("\n");
}
T_b("  ");
T_b("node = dci->");
T_b(te_extent->inactive);
T_b(".head;");
T_b("\n");
T_b("  ");
T_b("if ( 0 == node ) {");
T_b("\n");
if ( te_sys->UnitsToDynamicallyAllocate != 0 ) {
T_b("    ");
T_b(te_set->element_type);
T_b(" * container =");
T_b("\n");
T_b("      ");
T_b("( ");
T_b(te_set->element_type);
T_b(" *) ");
T_b(te_dma->allocate);
T_b("( ");
T_b(te_sys->UnitsToDynamicallyAllocate);
T_b(" * sizeof( ");
T_b(te_set->element_type);
T_b(" ) );");
T_b("\n");
T_b("    ");
T_b(te_instance->handle);
T_b(" pool = ( ");
T_b(te_instance->handle);
T_b(" ) ");
T_b(te_dma->allocate);
T_b("( ");
T_b(te_sys->UnitsToDynamicallyAllocate);
T_b(" * dci->");
T_b(te_extent->size_name);
T_b(" );");
T_b("\n");
T_b("    ");
T_b("if ( ( 0 == container ) || ( 0 == pool ) ) {");
T_b("\n");
T_b("      ");
T_b(te_callout->object_pool_empty);
T_b("( ");
T_b(domain_num_var);
T_b(", class_num );");
T_b("\n");
T_b("    ");
T_b("} else {");
T_b("\n");
T_b("      ");
T_b(te_string->memset);
T_b("( pool, 0, ");
T_b(te_sys->UnitsToDynamicallyAllocate);
T_b(" * dci->");
T_b(te_extent->size_name);
T_b(" );");
T_b("\n");
T_b("      ");
T_b("dci->");
T_b(te_extent->inactive);
T_b(".head = ");
T_b(te_set->insert_block);
T_b("(");
T_b("\n");
T_b("        ");
T_b("container, (const u1_t *) pool, dci->");
T_b(te_extent->size_name);
T_b(", ");
T_b(te_sys->UnitsToDynamicallyAllocate);
T_b(" );");
T_b("\n");
T_b("      ");
T_b("node = dci->");
T_b(te_extent->inactive);
T_b(".head;");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
} else {
T_b("    ");
T_b(te_callout->object_pool_empty);
T_b("( ");
T_b(domain_num_var);
T_b(", class_num );");
T_b("\n");
}
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("dci->");
T_b(te_extent->inactive);
T_b(".head = dci->");
T_b(te_extent->inactive);
T_b(".head->next;");
T_b("\n");
T_b("  ");
T_b("instance = (");
T_b(te_instance->handle);
T_b(") node->object;");
T_b("\n");
T_b("  ");
T_b("if ( 0 != dci->");
T_b(te_extent->istate_name);
T_b(" ) {");
T_b("\n");
T_b("    ");
T_b("instance->");
T_b(te_instance->current_state);
T_b(" = dci->");
T_b(te_extent->istate_name);
T_b(";");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
if ( te_sys->InstanceLoading ) {
T_b("  ");
T_b("if ( 0 == dci->active->head ) {");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->active);
T_b(".head = node;");
T_b("\n");
T_b("  ");
T_b("} else {");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->active);
T_b(".tail->next = node;");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("dci->");
T_b(te_extent->active);
T_b(".tail = node;");
T_b("\n");
T_b("  ");
T_b("node->next = 0;");
T_b("\n");
} else {
T_b("  ");
T_b(te_set->insert_instance);
T_b("( &dci->");
T_b(te_extent->active);
T_b(", node );");
T_b("\n");
}
if ( te_thread->enabled ) {
T_b("  ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_INSTANCE );");
T_b("\n");
}
T_b("  ");
T_b("return instance;");
T_b("\n");
T_b("}");
T_b("\n");
if ( te_sys->PersistentClassCount > 0 ) {
T_b(te_instance->handle);
T_b("\n");
T_b(te_instance->scope);
T_b(te_instance->create_persistent);
T_b("(");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(" domain_num,");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" class_num )");
T_b("\n");
T_b("{ /* Use this interface to create persistent instances.  */");
T_b("\n");
T_b("  ");
T_b(te_instance->handle);
T_b(" instance = ");
T_b(te_instance->scope);
T_b(te_instance->create);
T_b("( domain_num, class_num );");
T_b("\n");
T_b("  ");
T_b("/* Mark the persistent instance as \"clean\".  */");
T_b("\n");
T_b("  ");
T_b("instance->");
T_b(te_persist->dirty_name);
T_b(" = ");
T_b(te_persist->dirty_clean);
T_b(";");
T_b("\n");
T_b("  ");
T_b(persist_check_mark->name);
T_b("( instance, ");
T_b(domain_num_var);
T_b(", class_num );");
T_b("\n");
T_b("  ");
T_b("return instance;");
T_b("\n");
T_b("}");
T_b("\n");
}
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Delete an instance of the class passed and numbered on the input.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void");
T_b("\n");
T_b(te_instance->scope);
T_b(te_instance->delete);
T_b("(");
T_b("\n");
T_b("  ");
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
T_b(te_set->element_type);
T_b(" * node;");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = ");
if ( 0==strcmp("C++",te_target->language) ) {
T_b(te_instance->get_dci);
T_b("(class_num);");
T_b("\n");
} else {
T_b("*(");
T_b(te_cia->class_info_name);
T_b("[ ");
T_b(domain_num_var);
T_b(" ] + class_num);");
T_b("\n");
}
T_b("  ");
T_b("if ( 0 != instance ) {");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b(te_thread->mutex_lock);
T_b("( SEMAPHORE_FLAVOR_INSTANCE );");
T_b("\n");
}
if ( te_sys->CollectionsFlavor == 20 ) {
if ( te_sys->UnitsToDynamicallyAllocate != 0 ) {
T_b("    ");
T_b("node = ( ");
T_b(te_set->element_type);
T_b(" * ) ");
T_b(te_set->contains);
T_b("( &dci->");
T_b(te_extent->active);
T_b(", instance );");
T_b("\n");
} else {
T_b("    ");
T_b("node = &dci->");
T_b(te_extent->container_name);
T_b("[ 0 ] +");
T_b("\n");
T_b("      ");
T_b("(((char *) instance - (char *) dci->");
T_b(te_extent->pool_name);
T_b(" ) / dci->");
T_b(te_extent->size_name);
T_b(" );");
T_b("\n");
}
T_b("    ");
T_b(te_dlist->remove_node);
T_b("( &dci->");
T_b(te_extent->active);
T_b(", node );");
T_b("\n");
} else {
T_b("    ");
T_b("node = ");
T_b(te_slist->remove_node);
T_b("( &dci->active, instance );");
T_b("\n");
}
T_b("    ");
T_b("node->next = dci->");
T_b(te_extent->inactive);
T_b(".head;");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->inactive);
T_b(".head = node;");
T_b("\n");
T_b("    ");
T_b("/* Initialize storage to zero.  */");
T_b("\n");
T_b("    ");
T_b(te_string->memset);
T_b("( instance, 0, dci->");
T_b(te_extent->size_name);
T_b(" );");
T_b("\n");
T_b("    ");
T_b("if ( ( 0 != dci->");
T_b(te_extent->size_name);
T_b(" ) && ( 0 != dci->");
T_b(te_extent->istate_name);
T_b(" ) ) {");
T_b("\n");
T_b("      ");
T_b("instance->current_state = -1; /* 0xff max for error detection */");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
if ( te_thread->enabled ) {
T_b("    ");
T_b(te_thread->mutex_unlock);
T_b("( SEMAPHORE_FLAVOR_INSTANCE );");
T_b("\n");
}
T_b("  ");
T_b("}");
T_b("\n");
T_b("}");
T_b("\n");
if ( te_sys->PersistentClassCount > 0 ) {
T_b("void");
T_b("\n");
T_b(te_instance->scope);
T_b(te_instance->delete_persistent);
T_b("(");
T_b("\n");
T_b("  ");
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
T_b(" class_num )");
T_b("\n");
T_b("{ /* Use this interface to delete persistent instances.  */");
T_b("\n");
T_b("  ");
T_b(te_instance->scope);
T_b(te_instance->delete);
T_b("( instance, ");
T_b(domain_num_var);
T_b(", class_num );");
T_b("\n");
T_b("  ");
T_b("Escher_PersistDelete( instance, domain_num, class_num );");
T_b("\n");
T_b("}");
T_b("\n");
}
if ( te_sys->InstanceLoading ) {
T_b("typedef void (*brf)( Escher_iHandle_t );");
T_b("\n");
T_b("static brf batch_relaters[] = {");
T_b("\n");
T_b(all_batch_relaters);
T_b("");
T_b("};");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Self- inter-relate instances one to another.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void ");
T_b(te_prefix->result);
T_b("batch_relate( const ");
T_b(te_typemap->domain_number_name);
T_b(", const ");
T_b(te_typemap->object_number_name);
T_b(" );");
T_b("\n");
T_b("void ");
T_b(te_prefix->result);
T_b("batch_relate(");
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
T_b(te_typemap->instance_index_name);
T_b(" i;");
T_b("\n");
T_b("  ");
T_b(te_set->iterator_class_name);
T_b(" iterator;");
T_b("\n");
T_b("  ");
T_b(te_instance->handle);
T_b(" instance;");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = ");
if ( 0==strcmp("C++",te_target->language) ) {
T_b(te_instance->get_dci);
T_b("(class_num);");
T_b("\n");
} else {
T_b("*(");
T_b(te_cia->class_info_name);
T_b("[ domain_num ] + class_num);");
T_b("\n");
}
T_b("  ");
T_b(te_set->iterator_reset);
T_b("( &iterator, &dci->");
T_b(te_extent->active);
T_b(" );");
T_b("\n");
T_b("  ");
T_b("/* Cycle through the active list of instances of this class.  */");
T_b("\n");
T_b("  ");
T_b("while ( (instance = ");
T_b(te_set->iterator_next);
T_b("( &iterator )) != 0 ) {");
T_b("\n");
T_b("    ");
T_b("if ( 0 != batch_relaters[ class_num ] ) {");
T_b("\n");
T_b("      ");
T_b("(batch_relaters[ class_num ])( instance );");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("}");
T_b("\n");
T_b(all_instance_dumpersd);
T_b("");
T_b("static ");
T_b(te_prefix->result);
T_b("idf * instance_dumpers[ SYSTEM_DOMAIN_COUNT ] = {");
T_b("\n");
T_b(all_instance_dumpers);
T_b("");
T_b("};");
T_b("\n");
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Dump out instances of this class.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void ");
T_b(te_prefix->result);
T_b("dump_instances( const ");
T_b(te_typemap->domain_number_name);
T_b(", const ");
T_b(te_typemap->object_number_name);
T_b(" );");
T_b("\n");
T_b("void ");
T_b(te_prefix->result);
T_b("dump_instances(");
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
T_b(te_set->iterator_class_name);
T_b(" iterator;");
T_b("\n");
T_b("  ");
T_b(te_instance->handle);
T_b(" instance;");
T_b("\n");
T_b("  ");
T_b(te_prefix->result);
T_b("idf * instance_dumper = instance_dumpers[ domain_num ];");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = ");
if ( 0==strcmp("C++",te_target->language) ) {
T_b(te_instance->get_dci);
T_b("(class_num);");
T_b("\n");
} else {
T_b("*(");
T_b(te_cia->class_info_name);
T_b("[ domain_num ] + class_num);");
T_b("\n");
}
T_b("  ");
T_b("if ( 0 != *instance_dumper[ class_num ] ) {");
T_b("\n");
T_b("    ");
T_b(te_set->iterator_reset);
T_b("( &iterator, &dci->");
T_b(te_extent->active);
T_b(" );");
T_b("\n");
T_b("    ");
T_b("/* Cycle through the active list of instances of this class.  */");
T_b("\n");
T_b("    ");
T_b("while ( (instance = ");
T_b(te_set->iterator_next);
T_b("( &iterator )) != 0 ) {");
T_b("\n");
T_b("      ");
T_b("(*instance_dumper[ class_num ])( instance );");
T_b("\n");
T_b("    ");
T_b("}");
T_b("\n");
T_b("  ");
T_b("}");
T_b("\n");
T_b("}");
T_b("\n");
}
T_b("/*");
T_b("\n");
T_b(" ");
T_b("* Initialize object factory services.");
T_b("\n");
T_b(" ");
T_b("* Initialize class instance storage free pool (inanimate list)");
T_b("\n");
T_b(" ");
T_b("* by linking the empty instances into a collection.");
T_b("\n");
T_b(" ");
T_b("*/");
T_b("\n");
T_b("void");
T_b("\n");
T_b(te_instance->scope);
T_b(te_instance->factory_init);
T_b("(");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->domain_number_name);
T_b(" domain_num,");
T_b("\n");
T_b("  ");
T_b("const ");
T_b(te_typemap->object_number_name);
T_b(" class_num )");
T_b("\n");
T_b("{");
T_b("\n");
T_b("  ");
T_b(te_cia->class_info_type);
T_b(" * dci = ");
if ( 0==strcmp("C++",te_target->language) ) {
T_b(te_instance->get_dci);
T_b("(class_num);");
T_b("\n");
} else {
T_b("*(");
T_b(te_cia->class_info_name);
T_b("[ domain_num ] + class_num);");
T_b("\n");
}
T_b("  ");
T_b("if ( 0 != dci ) {");
T_b("\n");
if ( te_thread->flavor == "OSX" ) {
T_b("  ");
T_b("u1_t * zero = (u1_t *) dci->");
T_b(te_extent->pool_name);
T_b(";");
T_b("\n");
T_b("  ");
T_b("s4_t zi = dci->");
T_b(te_extent->population_name);
T_b(" * dci->");
T_b(te_extent->size_name);
T_b(";");
T_b("\n");
T_b("  ");
T_b("while ( 0 < zi-- ) { *zero++ = 0; }");
T_b("\n");
}
if ( te_sys->PEIClassCount > 0 ) {
T_b("    ");
T_b("int i = (intptr_t) dci->");
T_b(te_extent->active);
T_b(".head;");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->active);
T_b(".head = ");
T_b(te_set->insert_block);
T_b("(");
T_b("\n");
T_b("      ");
T_b("dci->");
T_b(te_extent->container_name);
T_b(",");
T_b("\n");
T_b("      ");
T_b("(const u1_t *) dci->");
T_b(te_extent->pool_name);
T_b(",");
T_b("\n");
T_b("      ");
T_b("dci->");
T_b(te_extent->size_name);
T_b(",");
T_b("\n");
T_b("      ");
T_b("i );");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->inactive);
T_b(".head = ");
T_b(te_set->insert_block);
T_b("(");
T_b("\n");
T_b("      ");
T_b("dci->");
T_b(te_extent->container_name);
T_b(" + i,");
T_b("\n");
T_b("      ");
T_b("(const u1_t *) dci->");
T_b(te_extent->pool_name);
T_b(" + ( i * dci->");
T_b(te_extent->size_name);
T_b(" ),");
T_b("\n");
T_b("      ");
T_b("dci->");
T_b(te_extent->size_name);
T_b(",");
T_b("\n");
T_b("      ");
T_b("dci->");
T_b(te_extent->population_name);
T_b(" - i );");
T_b("\n");
} else {
T_b("  ");
T_b("dci->");
T_b(te_extent->active);
T_b(".head = 0;");
T_b("\n");
T_b("  ");
T_b("dci->");
T_b(te_extent->inactive);
T_b(".head = ");
T_b(te_set->insert_block);
T_b("(");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->container_name);
T_b(",");
T_b("\n");
T_b("    ");
T_b("(const u1_t *) dci->");
T_b(te_extent->pool_name);
T_b(",");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->size_name);
T_b(",");
T_b("\n");
T_b("    ");
T_b("dci->");
T_b(te_extent->population_name);
T_b(" );");
T_b("\n");
}
T_b("  ");
T_b("}");
T_b("\n");
T_b("}");
T_b("\n");
T_b(te_file->arc_path);
