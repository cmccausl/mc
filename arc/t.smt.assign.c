.if ( "" != te_assign.array_spec )
  .if ( 4 == te_assign.Core_Typ )
    .// string
${ws}${te_instance.module}${te_string.strcpy}( ${te_assign.lval}, ${te_assign.rval} );
  .else
    .if ( 0 == te_assign.rval_dimensions )
${ws}${te_assign.lval} = ${te_assign.rval};
    .else
      .// We use memmove, because C does not copy arrays very nicely.
${ws}${te_instance.module}${te_string.memmove}( ${te_assign.lval}, ${te_assign.rval}, \
      .if ( is_parameter )
sizeof( ${te_assign.rval}[0] ) * ${element_count} );
      .else
sizeof( ${te_assign.rval} ) );
      .end if
    .end if
  .end if
.elif ( ( 9 == te_assign.Core_Typ ) or ( 21 == te_assign.Core_Typ ) )
${ws}${te_set.scope}${te_set.copy}( ${te_assign.lval}, ${te_assign.rval} );
.else
${ws}${te_assign.lval} = ${te_assign.rval};
.end if