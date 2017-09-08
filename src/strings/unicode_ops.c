/* Compares two strings, using the Unicode Collation Algorithm
 * Return values:
 *    0   The strings are identical for the collation levels requested
 * -1/1   String a is less than string b/String a is greater than string b
 *
 * `collation_mode` acts like a bitfield. Each of primary, secondary and tertiary
 * collation levels can be either: disabled, enabled, reversed.
 * In the table below, where + designates sorting normal direction and
 * - indicates reversed sorting for that collation level.
 *
 * Collation level | bitfield value
 *        Primary+ |   1
 *        Primary- |   2
 *      Secondary+ |   4
 *      Secondary- |   8
 *       Tertiary+ |  16
 *       Tertiary- |  32
 *     Quaternary+ |  64
 *     Quaternary- | 128
 */
//#define COLLATION_DEBUG 1
#ifdef COLLATION_DEBUG
#define dfprintf(...) fprintf (stderr, __VA_ARGS__)
#else
#define dfprintf(...)
#endif
MVMint32 MVM_unicode_collation_primary (MVMThreadContext *tc, MVMint32 codepoint) {
     return MVM_unicode_codepoint_get_property_int(tc, codepoint, MVM_UNICODE_PROPERTY_MVM_COLLATION_PRIMARY);
}
MVMint32 MVM_unicode_collation_secondary (MVMThreadContext *tc, MVMint32 codepoint) {
     return MVM_unicode_codepoint_get_property_int(tc, codepoint, MVM_UNICODE_PROPERTY_MVM_COLLATION_SECONDARY);
}
MVMint32 MVM_unicode_collation_tertiary (MVMThreadContext *tc, MVMint32 codepoint) {
     return MVM_unicode_codepoint_get_property_int(tc, codepoint, MVM_UNICODE_PROPERTY_MVM_COLLATION_TERTIARY);
}
MVMint32 MVM_unicode_collation_quickcheck (MVMThreadContext *tc, MVMint32 codepoint) {
    return MVM_unicode_codepoint_get_property_int(tc, codepoint, MVM_UNICODE_PROPERTY_MVM_COLLATION_QC);
}

struct collation_key_s {
    MVMuint32 primary, secondary, tertiary, index;
};
union collation_key_u {
    struct collation_key_s s;
    MVMuint32              a[4];
};
typedef union collation_key_u collation_key;
struct level_eval_s2 {
    MVMint32 Less, Same, More;
};
union level_eval_u2 {
    MVMint32 a2[3];
    struct level_eval_s2 s2;
};
struct level_eval_s {
    union level_eval_u2 primary, secondary, tertiary;
};
union level_eval_u {
    struct level_eval_s s;
    union  level_eval_u2 a[3];
};
typedef union level_eval_u level_eval;
#define initial_stack_size 100
#define collation_zero 1
struct collation_stack {
    collation_key *keys;
    MVMint64 stack_top;
    MVMint64 stack_size;
};
typedef struct collation_stack collation_stack;
static MVMint64 collation_push_cp (MVMThreadContext *tc, collation_stack *stack, MVMCodepointIter *ci, int *cp_maybe, int cp_num, char *name);
static void init_stack (MVMThreadContext *tc, collation_stack *stack) {
    stack->keys       = MVM_malloc(sizeof(collation_key) * initial_stack_size);
    stack->stack_top  = -1;
    stack->stack_size = initial_stack_size;
}
static void cleanup_stack (MVMThreadContext *tc, collation_stack *stack) {
    /* Must realloc to 0 in case we ended up resizing the stack */
    if (stack->stack_size != initial_stack_size)
        stack->keys = MVM_realloc(stack->keys, 0);
    /* If we get NULL then realloc has freed the memory.
     * If we have a pointer, either we didn't run realloc and need to free, or
     * realloc returned a pointer. realloc spec says if we get a pointer back we
     * must then free it using free. */
    if (stack->keys != NULL)
        MVM_free(stack->keys);
}
static void push_key_to_stack(collation_stack *stack, MVMuint32 primary, MVMuint32 secondary, MVMuint32 tertiary) {\
    stack->stack_top++;
    if (stack->stack_size <= stack->stack_top) {
        stack->keys = MVM_realloc(stack->keys,
            (stack->stack_size + initial_stack_size) * sizeof(collation_stack));
        stack->stack_size += initial_stack_size;
    }
    stack->keys[stack->stack_top].s.primary   = primary;
    stack->keys[stack->stack_top].s.secondary = secondary;
    stack->keys[stack->stack_top].s.tertiary  = tertiary;
}
static int print_sub_node (sub_node subnode) {
    dfprintf("{codepoint 0x%X, min 0x%X,max 0x%X,sub_node_elems %i,sub_node_link %i,collation_key_elems %i,collation_key_link %i}\n", subnode.codepoint, min(subnode), max(subnode), subnode.sub_node_elems, subnode.sub_node_link,
    subnode.collation_key_elems, subnode.collation_key_link);
    return 0;
}
static int print_stack (MVMThreadContext *tc, collation_stack *stack, char *name) {
    int i = 0;
    dfprintf("stack_%s print_stack() stack elems: %li\n", name, stack->stack_top + 1);
    for (i = 0; i < stack->stack_top + 1; i++) {
        dfprintf("stack_%s i: %i [%.4X.%.4X.%.4X]\n", name, i, stack->keys[i].s.primary, stack->keys[i].s.secondary, stack->keys[i].s.tertiary);
    }
    return 0;
}
static MVMint64 collation_push_level_separator (MVMThreadContext *tc, collation_stack *stack, char *name) {
    dfprintf("stack_%s collation_push_level_separator() level separator [%i.%i.%i] pushed onto stack_%s\n", name, collation_zero, collation_zero, 0, name);
    push_key_to_stack(stack, 0, 0, 0);
    dfprintf("stack_%s After collation_push_level_separator (maybe CLEAR)\n", name);
    print_stack(tc, stack, name);
    return 1;
}
/* Pushes collation keys from a collation_key struct and adds 1 to each level. (This is for places where
 * we store the native DUCET values and we add one because values on the stack are one more) */
static MVMint64 push_onto_stack (MVMThreadContext *tc, collation_stack *stack, collation_key *keys, int keys_to_push, char *name) {
    int j;
    dfprintf("stack_%s push_onto_stack() stack_top %li Before\n", name, stack->stack_top);
    print_stack(tc, stack, name);
    for (j = 0; j < keys_to_push; j++)
        push_key_to_stack(stack, keys[j].s.primary + 1, keys[j].s.secondary + 1, keys[j].s.tertiary + 1);

    dfprintf("stack_%s push_onto_stack() stack_top %li After\n", name, stack->stack_top);
    print_stack(tc, stack, name);
    return 1;
}
#define note_special_pushed(what, name, cp) {\
    dfprintf("PUSHED 0x%X %s onto stack_%s\n", cp, what, name);\
}
/* TODO write a script to generate this code */
MVM_STATIC_INLINE int is_unified_ideograph (MVMThreadContext *tc, int cp) {
    return (0x3400 <= cp && cp <= 0x4DB5) /*  3400..4DB5  d*/
    || (0x4E00 <= cp && cp <= 0x9FEA)     /*  4E00..9FEA  */
    || (0xFA0E <= cp && cp <= 0xFA0F)     /*  FA0E..FA0F  */
    || (cp == 0xFA11)                     /*  FA11        */
    || (0xFA13 <= cp && cp <= 0xFA14)     /*  FA13..FA14  */
    || (cp == 0xFA1F)                     /*  FA1F        */
    || (cp == 0xFA21)                     /*  FA21        */
    || (0xFA23  <= cp && cp <= 0xFA24)     /*  FA23..FA24  */
    || (0xFA27  <= cp && cp <= 0xFA29)     /*  FA27..FA29  */
    || (0x20000 <= cp && cp <= 0x2A6D6)   /* 20000..2A6D6 */
    || (0x2A700 <= cp && cp <= 0x2B734)   /* 2A700..2B734 */
    || (0x2B740 <= cp && cp <= 0x2B81D)   /* 2B740..2B81D */
    || (0x2B820 <= cp && cp <= 0x2CEA1)   /* 2B820..2CEA1 */
    || (0x2CEB0 <= cp && cp <= 0x2EBE0);  /* 2CEB0..2EBE0 */
}
MVM_STATIC_INLINE int compute_AAAA(MVMCodepoint cp, int offset) {
    return (offset + (cp >> 15));
}
MVM_STATIC_INLINE int compute_BBBB_offset(MVMCodepoint cp, int offset) {
    return ((cp - offset) | 0x8000);
}
MVM_STATIC_INLINE int compute_BBBB_and(MVMCodepoint cp) {
    return ((cp & 0x7FFF) | 0x8000);
}
MVM_STATIC_INLINE int is_Assigned_Block_Nushu(MVMCodepoint cp) {
    return (0x1B170 <= cp && cp <=0x1B2FF);
}
MVM_STATIC_INLINE int is_Block_Tangut(MVMCodepoint cp) {
    return (0x17000 <= cp && cp <= 0x18AFF);
}
MVM_STATIC_INLINE int is_Block_CJK_Unified_Ideographs_OR_CJK_Compatibility_Ideographs(MVMCodepoint cp) {
    return (0x4E00 <= cp && cp <= 0x9FFF)    /* 4E00..9FFF; CJK Unified Ideographs */
       ||  (0xF900 <= cp && cp <= 0xFAFF);  /* F900..FAFF; CJK Compatibility Ideographs */
}
static int NFD_and_push_collation_values (MVMThreadContext *tc, MVMCodepoint cp, collation_stack *stack, MVMCodepointIter *ci, char *name) {
    MVMNormalizer norm;
    MVMCodepoint cp_out;
    int ready, result_pos = 0;
    int got_ready;
    MVMCodepoint result[10];
    MVM_unicode_normalizer_init(tc, &norm, MVM_NORMALIZE_NFD);
    dfprintf("Going new path\n");
    ready = MVM_unicode_normalizer_process_codepoint(tc, &norm, cp, &cp_out);
    dfprintf("ready = %i\n", ready);
    if (ready) {
        result[result_pos++] = cp_out;
        while (0 < --ready)
            result[result_pos++] = MVM_unicode_normalizer_get_codepoint(tc, &norm);
    }
    MVM_unicode_normalizer_eof(tc, &norm);
    ready = MVM_unicode_normalizer_available(tc, &norm);
    dfprintf("Got ready2 = %i\n", ready);
    while (ready--)
        result[result_pos++] = MVM_unicode_normalizer_get_codepoint(tc, &norm);

    if (result[0] != cp || result_pos < 1) {
        for (ready = 0; ready < result_pos; ready++) {
            dfprintf("XXX Got codepoint 0x%X\n", result[ready]);
        }
        dfprintf("calling collation_push_cp on those new cp's\n");
        return collation_push_cp(tc, stack, ci, result, result_pos, name);
    }
    return 0;
}
/* Returns the number of collation elements pushed onto the stack */
static int collation_push_MVM_values (MVMThreadContext *tc, MVMCodepoint cp, collation_stack *stack, MVMCodepointIter *ci, char *name) {
    collation_key MVM_coll_key = {
        MVM_unicode_collation_primary(tc, cp), MVM_unicode_collation_secondary(tc, cp), MVM_unicode_collation_tertiary(tc, cp), 0
    };
    dfprintf("stack_%s in push_MVM_collatio_values()\n", name);
    if (MVM_coll_key.s.primary <= 0 || MVM_coll_key.s.secondary <= 0 || MVM_coll_key.s.tertiary <= 0) {
        MVMuint32 AAAA, BBBB;
        char *block_pushed = NULL;
        collation_key calculated_key[2] = {
            {0, 0x20, 0x2, 0},
            {0, 0x00, 0x0, 0}
        };
        /* Block=Tangut+Block=Tangut_Components 0x17000..0x18AFF */
        if (is_Block_Tangut(cp)) {
            AAAA = 0xFB00;
            BBBB = compute_BBBB_offset(cp, 0x17000);
            block_pushed = "Block_Tangut_and_Tangut_Components";
        }
        /* Assigned_Block=Nushu 0x1B170..1B2FF (*/
        else if (is_Assigned_Block_Nushu(cp)) {
            AAAA = 0xFB01;
            BBBB = compute_BBBB_offset(cp, 0x1B170);
            block_pushed = "Assigned_Block_Nushu";
        }
        /* Unified_Ideograph=True */
        else if (is_unified_ideograph(tc, cp)) {
            if (is_Block_CJK_Unified_Ideographs_OR_CJK_Compatibility_Ideographs(cp)) {
                AAAA = compute_AAAA(cp, 0xFB40);
                BBBB = compute_BBBB_and(cp);
                block_pushed = "Ideograph_CJK_Compatibility_OR_Unified";
            }
            /* All other Unified_Ideograph's */
            else {
                AAAA = compute_AAAA(cp, 0xFB80);
                BBBB = compute_BBBB_and(cp);
                block_pushed = "Ideograph_NOT_CJK_Compatibility_OR_Unified";
            }
        }
        else {
            int NFD_rtrn = NFD_and_push_collation_values(tc, cp, stack, ci, name);
            if (NFD_rtrn) {
                return NFD_rtrn;
            }
            else {
                AAAA = compute_AAAA(cp, 0xFBC0);
                BBBB = compute_BBBB_and(cp);
                block_pushed = "Unassigned";
            }
        }
        calculated_key[0].s.primary = AAAA;
        calculated_key[1].s.primary = BBBB;
        note_special_pushed(block_pushed, name, cp);
        push_onto_stack(tc, stack, calculated_key, 2, name);
        return 2;
    }
    else {
        dfprintf("Adding MVM_unicode_collation values to stack from cp 0x%X\n", cp);
        push_key_to_stack(stack,
            MVM_coll_key.s.primary,
            MVM_coll_key.s.secondary,
            MVM_coll_key.s.tertiary);
        return 1;
    }
}
/* This is passed the terminal node and it adds the collation elements linked from
 * that node to the collation stack
 * Returns:
    1 collation elements from last_node were used
    0 collation elements from the first node were used, or it fell back and used collation_push_MVM_values
 * Essentially the return value lets you know if it ended up pushing collation values for the last codepoint
 * in the sequence or if it only pushed collation values for fallback_cp
*/
MVMint64 collation_add_keys_from_node (MVMThreadContext *tc, sub_node *last_node, collation_stack *stack, MVMCodepointIter *ci, char *name, MVMCodepoint fallback_cp, sub_node *first_node) {
    MVMint64 j;
    MVMint64 rtrn = 0;
    sub_node *choosen_node = NULL;
    if (last_node)
        dfprintf("stack_%s in collation_add_keys_from_node() collation_key_link %i collation_key_elems %i\n", name, last_node->collation_key_link, last_node->collation_key_elems);
    /* If there are any collation elements */
    if (last_node && last_node->collation_key_elems) {
        choosen_node = last_node;
        rtrn = 1;
    }
    else if (first_node && first_node->collation_key_elems) {
        choosen_node = first_node;
    }
    if (choosen_node) {
        for (j = choosen_node->collation_key_link;
             j < choosen_node->collation_key_link + choosen_node->collation_key_elems;
             j++) {
            push_key_to_stack(stack, special_collation_keys[j].primary   + 1,
                special_collation_keys[j].secondary + 1,
                special_collation_keys[j].tertiary  + 1
            );
        }
        return rtrn;
    }
    dfprintf("Terminal node doesn't have any collation data. Using MVM_collation_values for fallback_cp 0x%X\n", fallback_cp);
    collation_push_MVM_values(tc, fallback_cp, stack, ci, name);
    return rtrn;
}
int find_next_node (MVMThreadContext *tc, sub_node node, MVMCodepoint next_cp) {
    MVMint64 next_node_min, next_node_max;
    MVMint64 i;
    /* There is nowhere else to go */
    if (!node.sub_node_elems)
        return -1;
    next_node_min = min(node);
    next_node_max = max(node);
    /* It's not within bounds */
    if (next_cp < next_node_min || next_node_max < next_cp)
        return -1;
    for (i = node.sub_node_link; i < node.sub_node_link + node.sub_node_elems; i++) {
        if (main_nodes[i].codepoint == next_cp)
            return i;
    }
    return -1;
}
int get_main_node (MVMThreadContext *tc, int cp, int range_min, int range_max) {
    MVMint64 i;
    MVMint64 rtrn = -1;
    int counter   = 0;
    /* Decrement range_min because binary search defaults to 1..* not 0..* */
    range_min--;
    dfprintf("starter_main_nodes_elems = %i\n", starter_main_nodes_elems);
    dfprintf("lowest/highest starter nodes codepoints: %i/%i\n", main_nodes[0].codepoint, main_nodes[starter_main_nodes_elems - 1].codepoint);
    dfprintf("range_min = %i range_max = %i\n", range_min, range_max);
    for (range_min = -1, range_max = starter_main_nodes_elems; 1 < range_max - range_min;) {
        i = (range_min + range_max) / 2;
        dfprintf("Setting i to %"PRIi64"\n", i);
        dfprintf("Checking node %"PRIi64" codepoint = %i\n", i, main_nodes[i].codepoint);
        if (cp  <= main_nodes[i].codepoint) {
            dfprintf("cp< Setting range_max to i → %"PRIi64" before range_min = %i range_max = %i\n", i, range_min, range_max);
            range_max = i;
        }
        else {
            dfprintf("cp> Setting range_min to i → %"PRIi64" before range_min = %i range_max = %i\n", range_min, range_min, range_max);
            range_min = i;
        }
    }
    dfprintf("Final check: node: %i codepoint: %i\n", range_max, main_nodes[range_max].codepoint);
    if (main_nodes[range_max].codepoint == cp)
        rtrn = range_max;
    dfprintf("Returning i as %i\n", -1);
    return rtrn;
}
void print_cps_array (MVMCodepoint *cps, int start_at, MVMCodepoint cp_num) {
    MVMint64 j;
    dfprintf("print_cps_array() called\n");
    for (j = start_at; j <= cp_num; j++) {
        dfprintf("cps[%"PRIi64"i] 0x%X ", j, cps[j]);
    }
    dfprintf("\n");
}
#define collation_push_from_iter -1
/* Returns the number of added collation keys */
static MVMint64 collation_push_cp (MVMThreadContext *tc, collation_stack *stack, MVMCodepointIter *ci, int *cp_maybe, int cp_num, char *name) {
    MVMint64 rtrn = 0;
    MVMCodepoint cps[10];
    MVMint64 num_cps_processed = 0;
    int query = -1;
    int cp_num_orig = cp_num;
    /* If supplied -1 that means we need to grab it from the codepoint iterator. Otherwise
     * the value we were passed is the codepoint we should proces */
    if (cp_num == 0) {
        cps[0] = MVM_string_ci_get_codepoint(tc, ci);
        cp_num = 1;
        dfprintf("stack_%s collation_push_cp grabbing codepoint 0x%X from iterator\n",name, cps[0]);
    }
    else {
        int i;
        dfprintf("stack_%s collation_push_cp was supplied cp_num: %i codepoints ", name, cp_num);
        for (i = 0; i < cp_num; i++) {
            cps[i] = cp_maybe[i];
            dfprintf("0x%X ", cps[i]);
        }
        dfprintf("\n");
    }
    dfprintf("push orig stack_top %li. get_main_node(cps[0]) returned with %i\n", stack->stack_top, query);
    query = get_main_node(tc, cps[0], 0, starter_main_nodes_elems);
    if (query != -1) {
        dfprintf("query != -1 (%i) stack_%s getting value from special_collation_keys\n", query, name);
        /* If there are no sub_node_elems that means we don't need to look at
         * the next codepoint, we are already at the correct node
         * If there's no more codepoints in the iterator we also are done here */
        if (main_nodes[query].sub_node_elems < 1 || cp_num < 2 && !MVM_string_ci_has_more(tc, ci)) {
            dfprintf("stack_%s main_node collation_key_link %i collation_key_elems %i\n", name, main_nodes[query].collation_key_link, main_nodes[query].collation_key_elems);
            collation_add_keys_from_node(tc, NULL, stack, ci, name, cps[0],  &main_nodes[query]);
            num_cps_processed++;
        }
        /* Otherwise we need to check the next codepoint(s) (0 < sub_node_elems) */
        else {
            int i, result = query;
            int last_good_i = 0;
            int last_good_result = -1;
            dfprintf("stack_%s main node: ", name);
            print_sub_node(main_nodes[query]);
            for (i = 0; result != -1 && MVM_string_ci_has_more(tc, ci) && i < 10;) {
                i++;
                /* Only grab a codepoint if it doesn't already exist in the array */
                if (cp_num <= i) {
                    cps[i] = MVM_string_ci_get_codepoint(tc, ci);
                    cp_num++;
                    dfprintf("grabbed cp\n");
                }
                else {
                    dfprintf("existing cp\n");
                }
                dfprintf("cps[++i] -> cps[%i] = MVM_string_ci_get_codepoint = 0x%X. result: %i\n", i, cps[i], result);
                result = find_next_node(tc, main_nodes[result], cps[i]);
                dfprintf("result %i i %i\n", result, i);
                /* If we got something other than -1 and it has collation elements
                 * store the value so we know how far is valid */
                if (result != -1 && main_nodes[result].collation_key_elems != 0) {
                    last_good_i      = i;
                    last_good_result = result;
                }
                if (result != -1)
                    print_sub_node(main_nodes[result]);
            }
            dfprintf("stack_%s last_good_i %i i=%i\n", name, last_good_i, i);
            dfprintf("stack_%s settled on : ", name);
            /* If there is no last_good_result we should return a value from main_nodes */
            print_sub_node( (last_good_result == -1 ? main_nodes[query] : main_nodes[last_good_result]) );
            /* If the terminal_subnode can't be processed then that means it will push the starter codepoint ( cp[0] )'s value onto
             * the stack, and we must set last_good_i to 0 since it didn't work out */
            if (!collation_add_keys_from_node(tc, (last_good_result == -1 ? NULL : &main_nodes[last_good_result]), stack, ci, name, cps[0], &main_nodes[query])) {
                /* If we get 0 from collation_add_keys_from_node then we only processed
                 * a single codepoint so set last_good_i to 0 */
                last_good_i = 0;
            }
            dfprintf("On the stack have: ");
            print_cps_array(cps, last_good_i + 1, i);
            dfprintf("\n");
            num_cps_processed = last_good_i + 1;
        }
    }
    else {
        /* Push the first codepoint onto the stack */
        rtrn = collation_push_MVM_values(tc, cps[0], stack, ci, name);
        num_cps_processed = 1;
    }
    dfprintf("stack_%s num_cps_processed = %"PRIi64" cp_num = %i\n", name, num_cps_processed, cp_num);
    /* If there are any more codepoints remaining call collation_push_cp on the remaining */
    if (num_cps_processed < cp_num) {
        return num_cps_processed + collation_push_cp(tc, stack, ci, cps + num_cps_processed, cp_num - num_cps_processed, name);
    }
    return num_cps_processed;
}
MVMint64 grab_from_stack(MVMThreadContext *tc, MVMCodepointIter *ci,
    collation_stack *stack, char *name) {

    dfprintf("Grabbing from the stack. stack_%s stack_elems %li\n", name, stack->stack_top + 1);
    if (!MVM_string_ci_has_more(tc, ci))
        return 0;
    collation_push_cp(tc, stack, ci, NULL, 0, name);
    dfprintf("Finished one grapheme grab to stack. stack_%s stack_elems %li\n", name, stack->stack_top + 1);
    return 1;
}
#define codepoint_sequence_no_max 3
struct ring_buffer {
    MVMCodepoint codes[codepoint_sequence_no_max];
    MVMuint32    count;
    MVMint32  location;
    MVMCodepoint codes_out[codepoint_sequence_no_max];
    MVMuint32    codes_out_count;
};
typedef struct ring_buffer ring_buffer;
static void init_ringbuffer (MVMThreadContext *tc,  ring_buffer *buffer) {
    MVMint64 i;
    /* TODO remove this loop which isn't needed when merging to master */
    for (i = 0; i < codepoint_sequence_no_max; i++) {
        buffer->codes[i]     = -1;
        buffer->codes_out[i] = -1;
    }
    buffer->count           =  0;
    buffer->location        = -1;
    buffer->codes_out_count =  0;
}
static void add_to_ring_buffer(MVMThreadContext *tc, ring_buffer *buffer, MVMCodepoint cp) {
    buffer->location++;
    dfprintf("Adding 0x%X to ring buffer\n", cp);
    if (codepoint_sequence_no_max <= buffer->location)
        buffer->location = 0;
    buffer->codes[buffer->location] = cp;
    buffer->count++;
}
static void print_ring_buffer(MVMThreadContext *tc, ring_buffer *buffer) {
    MVMint64 i;
    dfprintf("Buffer: count: %"PRIu32" location %"PRIi32"\nBuffer contents: ", buffer->count, buffer->location);
    for (i = 0; i < buffer->count && i < codepoint_sequence_no_max; i++) {
        dfprintf("i: %"PRIi64": cp: 0x%X ", i, buffer->codes[i]);
    }
    dfprintf("\n");
}
/* Takes the ring buffer and puts the codepoints in order */
static void ring_buffer_done(MVMThreadContext *tc, ring_buffer *buffer) {
    buffer->codes_out_count = codepoint_sequence_no_max < buffer->count ? codepoint_sequence_no_max : buffer->count;
    /* If the buffer hasn't been wraped yet or the last codepoint was pushed onto the last buffer element,
     * use memcpy to copy it to the out buffer */
    if (buffer->count <= codepoint_sequence_no_max || buffer->location == codepoint_sequence_no_max - 1) {
        dfprintf("using memcpy to copy buffer\n");
        memcpy(buffer->codes_out, buffer->codes, sizeof(MVMCodepoint) * buffer->codes_out_count);
    }
    /* Otherwise we need to copy it manually */
    else {
        /* Copy backwards from the last copied to the first copied codepoint in the ring buffer */
        MVMint32 out_location     = buffer->codes_out_count - 1;
        MVMint32 buf_location     = buffer->location;
        for (; 0 <= out_location; out_location--) {
            dfprintf("buffer-codes_out[%i] = 0x%X\n", out_location, buffer->codes[buf_location]);
            buffer->codes_out[out_location] = buffer->codes[buf_location];
            buf_location--;
            if (buf_location < 0)
                buf_location = codepoint_sequence_no_max - 1;
        }
    }

}
static MVMint64 collation_return_by_length(MVMThreadContext *tc, MVMStringIndex length_a, MVMStringIndex length_b, MVMint64 collation_mode) {
    /* Quaternary both enabled and reversed. This cancels out and it is ignored */
    if (collation_mode & 64 && collation_mode & 128) {
        return 0;
    }
    /* Quaternary+ */
    else if (collation_mode & 64) {
        return length_a < length_b ? -1 :
               length_b < length_a ?  1 :
                                      0 ;
    }
    /* Quaternary- */
    else if (collation_mode & 128) {
        return length_a < length_b ?  1 :
               length_b < length_a ? -1 :
                                      0 ;
    }
    /* Quaternary level null */
    return 0;
}
/* MVM_unicode_string_compare implements the Unicode Collation Algorthm */
MVMint64 MVM_unicode_string_compare(MVMThreadContext *tc, MVMString *a, MVMString *b,
         MVMint64 collation_mode, MVMint64 lang_mode, MVMint64 country_mode) {
    MVMStringIndex alen, blen;
    /* Iteration variables */
    MVMCodepointIter a_ci, b_ci;
    MVMGrapheme32 ai, bi;
    /* Set it all to 0 to start with. We alter this based on the collation_mode later on */
    level_eval level_eval_settings = {
        { {0,0,0}, {0,0,0}, {0,0,0} }
    };
    /* The default level_eval settings, used between two non-equal levels */
    union level_eval_u2 level_eval_default = {
        {-1, 0, 1}
    };
    /* Collation stacks */
    collation_stack stack_a;
    collation_stack stack_b;

    ring_buffer buf_a, buf_b;
    /* This value stores what the return value would be if the strings were compared
     * by codepoint. This is used to break collation value ties */
    MVMint64 compare_by_cp_rtrn = 0;
    MVMint64 pos_a = 0, pos_b = 0, i = 0, rtrn = 0;
    MVMint16 grab_a_done = 0, grab_b_done = 0;
    /* From 0 to 2 for primary, secondary, tertiary levels */
    MVMint16   level_a = 0,   level_b = 0;
    MVMint64 skipped_a = 0, skipped_b = 0;
    /* This code sets up level_eval_settings based on the collation_mode */
    #define setmodeup(mode, level, Less, Same, More) {\
        if (collation_mode & mode) {\
            level_eval_settings.a[level].a2[0] +=  Less;\
            level_eval_settings.a[level].a2[1] +=  Same;\
            level_eval_settings.a[level].a2[2] +=  More;\
        }\
    }
    /* Primary */
    setmodeup( 1, 0,   -1, 0,  1);
    setmodeup( 2, 0,    1, 0, -1);
    /* Secondary */
    setmodeup( 4, 1,   -1, 0,  1);
    setmodeup( 8, 1,    1, 0, -1);
    /* Tertiary */
    setmodeup(16, 2,   -1, 0,  1);
    setmodeup(32, 2,    1, 0, -1);

    dfprintf("Setting: %li\n", collation_mode);
    dfprintf("Setting primary   {%i,%i,%i}\n", level_eval_settings.a[0].a2[0], level_eval_settings.a[0].a2[1], level_eval_settings.a[0].a2[2]);
    dfprintf("Setting secondary {%i,%i,%i}\n", level_eval_settings.a[1].a2[0], level_eval_settings.a[1].a2[1], level_eval_settings.a[1].a2[2]);
    dfprintf("Setting tertiary  {%i,%i,%i}\n", level_eval_settings.a[2].a2[0], level_eval_settings.a[2].a2[1], level_eval_settings.a[2].a2[2]);

    init_stack(tc, &stack_a);
    init_stack(tc, &stack_b);
    MVM_string_check_arg(tc, a, "compare");
    MVM_string_check_arg(tc, b, "compare");
    /* Simple cases when one or both are zero length. */
    alen = MVM_string_graphs_nocheck(tc, a);
    blen = MVM_string_graphs_nocheck(tc, b);
    if (alen == 0 || blen == 0)
        return collation_return_by_length(tc, alen, blen, collation_mode);

    /* Initialize a codepoint iterator */
    MVMROOT(tc, a_ci, {
        MVM_string_ci_init(tc, &a_ci, a, 0);
        MVMROOT(tc, b_ci, {
            MVM_string_ci_init(tc, &b_ci, b, 0);
        });
    });
    MVM_string_ci_init(tc, &a_ci, a, 0);
    MVM_string_ci_init(tc, &b_ci, b, 0);
    init_ringbuffer(tc, &buf_a);
    init_ringbuffer(tc, &buf_b);
    /* The ring buffers hold the exact number of codepoints which comprise the longest
     * sequence of codepoints which map to its own collation keys in the Unicode
     * Collation Algorithm. As of Unicode 9.0 this number was 3. The number is
     * generated by the script that generates the C data so we only need to retain
     * that many codepoints. TODO actually generate codepoint_sequence_no_max */
    while (MVM_string_ci_has_more(tc, &a_ci) && MVM_string_ci_has_more(tc, &b_ci)) {
        MVMCodepoint cp_a = MVM_string_ci_get_codepoint(tc, &a_ci);
        MVMCodepoint cp_b = MVM_string_ci_get_codepoint(tc, &b_ci);
        add_to_ring_buffer(tc, &buf_a, cp_a);
        add_to_ring_buffer(tc, &buf_b, cp_b);
        if (cp_a != cp_b) {
            compare_by_cp_rtrn = cp_a < cp_b ? -1 :
                                 cp_b < cp_a ?  1 :
                                                0 ;
            break;
        }
    }
    print_ring_buffer(tc, &buf_a);
    print_ring_buffer(tc, &buf_b);

    ring_buffer_done(tc, &buf_a);
    ring_buffer_done(tc, &buf_b);
    /* Otherwise, need to iterate by codepoint */
    dfprintf("Pushing initial collation elements to the stack\n");
    collation_push_cp(tc, &stack_b, &b_ci, buf_b.codes_out, buf_b.codes_out_count, "b");
    dfprintf("After Initial grab b\n");
    print_stack(tc, &stack_b, "b");
    collation_push_cp(tc, &stack_a, &a_ci, buf_a.codes_out, buf_a.codes_out_count, "a");
    dfprintf("After Initial grab a\n");
    print_stack(tc, &stack_a, "a");

    while (rtrn == 0) {
        while (pos_a <= stack_a.stack_top && pos_b <= stack_b.stack_top) {
            dfprintf("stack_a index %"PRIi64" is value %X\n", pos_a, stack_a.keys[pos_a].s.primary);
            dfprintf("stack_b index %"PRIi64" is value %X\n", pos_b, stack_b.keys[pos_b].s.primary);

            dfprintf(">> checking level_a %"PRIi16" at pos_a %"PRIi64" level_b %"PRIi16" at pos_b %"PRIi64"\n", level_a, pos_a, level_b, pos_b);
            /* Collation values are set as 1 (collation_zero) higher than what Unicode designates. So a collation value of 1 is ignored as
             * a 0 in DUCET would be. Whereas a collation value of 0 cannot be skipped and is used only for the tertiary level
             * of a level separator so it is evaluated as the end of the string, causing the shorter string to win */
            if (stack_a.keys[pos_a].a[level_a] == collation_zero) {
                pos_a++;
                skipped_a++;
                dfprintf("Skipping a because it was 0001 in value\n");
                continue;
            }
            if (stack_b.keys[pos_b].a[level_b] == collation_zero) {
                pos_b++;
                skipped_b++;
                dfprintf("Skipping b because it was 0001 in value\n");
                continue;
            }
            dfprintf("stack_a pos_a %"PRIi64" level_a %"PRIi16" = %i. stack_b pos_b %"PRIi64" level_b %"PRIi16" = %i\n",
                pos_a, level_a, stack_a.keys[pos_a].a[level_a],
                pos_b, level_b, stack_b.keys[pos_b].a[level_b]);
            /* If collation values are not equal */
            if (stack_a.keys[pos_a].a[level_a] != stack_b.keys[pos_b].a[level_b]) {
                union level_eval_u2 effective_level_eval = level_eval_default;
                /* Aside from ignored (collation_zero) levels, when all primary values
                 * are greater than any secondary values. All secondary values are greater
                 * than any tertiary values. Because of this, we only need to tailor effective_level_evals if the levels match */
                if (level_a == level_b)
                    effective_level_eval = level_eval_settings.a[level_a];
                rtrn = stack_a.keys[pos_a].a[level_a] < stack_b.keys[pos_b].a[level_b] ?  effective_level_eval.s2.Less :
                       stack_a.keys[pos_a].a[level_a] > stack_b.keys[pos_b].a[level_b] ?  effective_level_eval.s2.More :
                                                                                          effective_level_eval.s2.Same ;
                  print_stack(tc, &stack_a, "a"); print_stack(tc, &stack_b, "b");
            }
            if (rtrn != 0) {
                dfprintf("\nDONE decided rtrn=%"PRIi64"\npos_a=%"PRIi64" pos_b=%"PRIi64" a_keys_pushed=%"PRIi64" b_keys_pushed=%"PRIi64"\n", rtrn, pos_a, pos_b, stack_a.stack_top + 1, stack_b.stack_top + 1);
                cleanup_stack(tc, &stack_a);
                cleanup_stack(tc, &stack_b);
                return rtrn;
            }
            pos_a++;
            pos_b++;
        }
        #define if_grab_done(grab_done, stack, ci, pos, level, name) {\
            if (!grab_done) {\
                dfprintf("Pushing %s NON_INITIAL collation elements to the stack\n", name);\
                if (!grab_from_stack(tc, &ci, &stack, name)) {\
                    dfprintf("Pushing level separator to stack_%s\n", name);\
                    collation_push_level_separator(tc, &stack, name);\
                    grab_done = 1;\
                }\
            }\
            /* Here we wrap to the next level of collation elements */\
            if (grab_done && stack.stack_top < pos) {\
                if (level < 2) {\
                    pos = 0;\
                    level++;\
                    dfprintf("Setting level_%s to %"PRIi16" and pos_%s to %"PRIi64". %s_keys_pushed: %li\n",\
                                            name,      level,   name, pos,name, stack.stack_top + 1);\
                }\
                else {\
                    dfprintf("Can't wrap string_%s anymore so breaking\n", name);\
                    print_stack(tc, &stack_a, "a"); print_stack(tc, &stack_b, "b");\
                    break;\
                }\
            }\
        }
        /* Here we wrap to the next level of collation elements if needed */
        if_grab_done(grab_b_done, stack_b, b_ci, pos_b, level_b, "b");
        if_grab_done(grab_a_done, stack_a, a_ci, pos_a, level_a, "a");
    }
    cleanup_stack(tc, &stack_a);
    cleanup_stack(tc, &stack_b);
    dfprintf("towards the end\n");
    /* If we get here, they tied for all levels including the level separator
     * [0001.0001.0000] The primary and secondary of the level separator we push
     * get ignored, but the tertiary level value 0 is not ignored. No other values
     * have 0, so that means those levels must have matched up.
     * Return based on the number of 0001 values we skipped instead to make those
     * win */
    if (skipped_a != skipped_b) {
        dfprintf("Returning by number of skipped. skipped_a %"PRIi64" skipped_b %"PRIi64"\n", skipped_a, skipped_b);
        return stack_a.stack_top < stack_b.stack_top ? -1 :
               stack_b.stack_top < stack_a.stack_top ?  1 :
                                                        0 ;
    }

    /* If we get here then the tie must be broken by codepoint. Use the return
     * value we computed at the beginning of the function when we were pushing
     * onto the ring buffers */
    if (compare_by_cp_rtrn) {
        dfprintf("Returning based on compare_by_cp_rtrn\n");
        return compare_by_cp_rtrn;
    }
    dfprintf("Returning by collation_return_by_length\n");
    /* If we get here, the two strings are equal for the length of the shorter
     * string. Return based on length */
    return collation_return_by_length(tc, alen, blen, collation_mode);
}

/* Looks up a codepoint by name. Lazily constructs a hash. */
MVMGrapheme32 MVM_unicode_lookup_by_name(MVMThreadContext *tc, MVMString *name) {
    MVMuint64 size;
    char *cname = MVM_string_utf8_encode_C_string(tc, name);
    size_t cname_len = strlen((const char *) cname );
    MVMUnicodeNameRegistry *result;
    if (!codepoints_by_name) {
        generate_codepoints_by_name(tc);
    }
    HASH_FIND(hash_handle, codepoints_by_name, cname, cname_len, result);

    MVM_free(cname);
    return result ? result->codepoint : -1;
}

MVMString * MVM_unicode_get_name(MVMThreadContext *tc, MVMint64 codepoint) {
    const char *name = NULL;

    /* Catch out-of-bounds code points. */
    if (codepoint < 0) {
        name = "<illegal>";
    }
    else if (0x10ffff < codepoint) {
        name = "<unassigned>";
    }

    /* Look up name. */
    else {
        MVMuint32 codepoint_row = MVM_codepoint_to_row_index(tc, codepoint);
        if (codepoint_row != -1) {
            name = codepoint_names[codepoint_row];
            if (!name) {
                while (codepoint_row && !codepoint_names[codepoint_row])
                    codepoint_row--;
                name = codepoint_names[codepoint_row];
                if (!name || name[0] != '<')
                    name = "<reserved>";
            }
        }
        else {
            name = "<illegal>";
        }
    }

    return MVM_string_ascii_decode(tc, tc->instance->VMString, name, strlen(name));
}

MVMString * MVM_unicode_codepoint_get_property_str(MVMThreadContext *tc, MVMint64 codepoint, MVMint64 property_code) {
    const char * const str = MVM_unicode_get_property_str(tc, codepoint, property_code);

    if (!str)
        return tc->instance->str_consts.empty;

    return MVM_string_ascii_decode(tc, tc->instance->VMString, str, strlen(str));
}

const char * MVM_unicode_codepoint_get_property_cstr(MVMThreadContext *tc, MVMint64 codepoint, MVMint64 property_code) {
    return MVM_unicode_get_property_str(tc, codepoint, property_code);
}

MVMint64 MVM_unicode_codepoint_get_property_int(MVMThreadContext *tc, MVMint64 codepoint, MVMint64 property_code) {
    if (property_code == 0)
        return 0;
    return (MVMint64)MVM_unicode_get_property_int(tc, codepoint, property_code);
}

MVMint64 MVM_unicode_codepoint_get_property_bool(MVMThreadContext *tc, MVMint64 codepoint, MVMint64 property_code) {
    if (property_code == 0)
        return 0;
    return (MVMint64)MVM_unicode_get_property_int(tc, codepoint, property_code) != 0;
}

MVMint64 MVM_unicode_codepoint_has_property_value(MVMThreadContext *tc, MVMint64 codepoint, MVMint64 property_code, MVMint64 property_value_code) {
    if (property_code == 0)
        return 0;
    return (MVMint64)MVM_unicode_get_property_int(tc,
        codepoint, property_code) == property_value_code ? 1 : 0;
}

/* Looks if there is a case change for the provided codepoint. Since a case
 * change may produce multiple codepoints occasionally, then we return 0 if
 * the case change is a no-op, and otherwise the number of codepoints. The
 * codepoints argument will be set to a pointer to a buffer where those code
 * points can be read from. The caller must not mutate the buffer, nor free
 * it. */
MVMuint32 MVM_unicode_get_case_change(MVMThreadContext *tc, MVMCodepoint codepoint, MVMint32 case_,
                                      const MVMCodepoint **result) {
    if (case_ == MVM_unicode_case_change_type_fold) {
        MVMint32 folding_index = MVM_unicode_get_property_int(tc,
            codepoint, MVM_UNICODE_PROPERTY_CASE_FOLDING);
        if (folding_index) {
            MVMint32 is_simple = MVM_unicode_get_property_int(tc,
                codepoint, MVM_UNICODE_PROPERTY_CASE_FOLDING_SIMPLE);
            if (is_simple) {
                *result = &(CaseFolding_simple_table[folding_index]);
                return 1;
            }
            else {
                MVMint32 i = 3;
                while (0 < i && CaseFolding_grows_table[folding_index][i - 1] == 0)
                    i--;
                *result = &(CaseFolding_grows_table[folding_index][0]);
                return i;
            }
        }
    }
    else {
        MVMint32 special_casing_index = MVM_unicode_get_property_int(tc,
            codepoint, MVM_UNICODE_PROPERTY_SPECIAL_CASING);
        if (special_casing_index) {
            MVMint32 i = 3;
                while (0 < i && SpecialCasing_table[special_casing_index][case_][i - 1] == 0)
                    i--;
                *result = SpecialCasing_table[special_casing_index][case_];
                return i;
        }
        else {
            MVMint32 changes_index = MVM_unicode_get_property_int(tc,
                codepoint, MVM_UNICODE_PROPERTY_CASE_CHANGE_INDEX);
            if (changes_index) {
                const MVMCodepoint *found = &(case_changes[changes_index][case_]);
                if (*found != 0) {
                    *result = found;
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* XXX make all the statics members of the global MVM instance instead? */
static MVMUnicodeNameRegistry *property_codes_by_names_aliases;
static MVMUnicodeGraphemeNameRegistry *property_codes_by_seq_names;

static void generate_property_codes_by_names_aliases(MVMThreadContext *tc) {
    MVMuint32 num_names = num_unicode_property_keypairs;

    while (num_names--) {
        MVMUnicodeNameRegistry *entry = MVM_malloc(sizeof(MVMUnicodeNameRegistry));
        entry->name = (char *)unicode_property_keypairs[num_names].name;
        entry->codepoint = unicode_property_keypairs[num_names].value;
        HASH_ADD_KEYPTR(hash_handle, property_codes_by_names_aliases,
            entry->name, strlen(entry->name), entry);
    }
}
static void generate_property_codes_by_seq_names(MVMThreadContext *tc) {
    MVMuint32 num_names = num_unicode_seq_keypairs;

    while (num_names--) {
        MVMUnicodeGraphemeNameRegistry *entry = MVM_malloc(sizeof(MVMUnicodeGraphemeNameRegistry));
        entry->name = (char *)uni_seq_pairs[num_names].name;
        entry->structindex = uni_seq_pairs[num_names].value;
        HASH_ADD_KEYPTR(hash_handle, property_codes_by_seq_names,
            entry->name, strlen(entry->name), entry);
    }
}

MVMint32 MVM_unicode_name_to_property_code(MVMThreadContext *tc, MVMString *name) {
    MVMuint64 size;
    char *cname = MVM_string_ascii_encode(tc, name, &size, 0);
    MVMUnicodeNameRegistry *result;
    if (!property_codes_by_names_aliases) {
        generate_property_codes_by_names_aliases(tc);
    }
    HASH_FIND(hash_handle, property_codes_by_names_aliases, cname, strlen((const char *)cname), result);
    MVM_free(cname); /* not really codepoint, really just an index */
    return result ? result->codepoint : 0;
}

static void generate_unicode_property_values_hashes(MVMThreadContext *tc) {
    MVMUnicodeNameRegistry **hash_array = MVM_calloc(MVM_NUM_PROPERTY_CODES, sizeof(MVMUnicodeNameRegistry *));
    MVMuint32 index = 0;
    MVMUnicodeNameRegistry *entry = NULL, *binaries = NULL;
    for ( ; index < num_unicode_property_value_keypairs; index++) {
        MVMint32 property_code = unicode_property_value_keypairs[index].value >> 24;
        entry = MVM_malloc(sizeof(MVMUnicodeNameRegistry));
        entry->name = (char *)unicode_property_value_keypairs[index].name;
        entry->codepoint = unicode_property_value_keypairs[index].value & 0xFFFFFF;
        HASH_ADD_KEYPTR(hash_handle, hash_array[property_code],
            entry->name, strlen(entry->name), entry);
    }
    for (index = 0; index < MVM_NUM_PROPERTY_CODES; index++) {
        if (!hash_array[index]) {
            if (!binaries) {
                MVMUnicodeNamedValue yes[8] = { {"T",1}, {"Y",1},
                    {"Yes",1}, {"yes",1}, {"True",1}, {"true",1}, {"t",1}, {"y",1} };
                MVMUnicodeNamedValue no [8] = { {"F",0}, {"N",0},
                    {"No",0}, {"no",0}, {"False",0}, {"false",0}, {"f",0}, {"n",0} };
                MVMuint8 i;
                for (i = 0; i < 8; i++) {
                    entry = MVM_malloc(sizeof(MVMUnicodeNameRegistry));
                    entry->name = (char *)yes[i].name;
                    entry->codepoint = yes[i].value;
                    HASH_ADD_KEYPTR(hash_handle, binaries, yes[i].name, strlen(yes[i].name), entry);
                }
                for (i = 0; i < 8; i++) {
                    entry = MVM_malloc(sizeof(MVMUnicodeNameRegistry));
                    entry->name = (char *)no[i].name;
                    entry->codepoint = no[i].value;
                    HASH_ADD_KEYPTR(hash_handle, binaries, no[i].name, strlen(no[i].name), entry);
                }
            }
            hash_array[index] = binaries;
        }
    }
    unicode_property_values_hashes = hash_array;
}
/* Quickly determines the length of a number 6.5x faster than doing log10 after
 * compiler optimization */
MVM_STATIC_INLINE size_t length_of_num (size_t number) {
    if (number < 10) return 1;
    return 1 + length_of_num(number / 10);
}
MVMint32 unicode_cname_to_property_value_code(MVMThreadContext *tc, MVMint64 property_code, const char *cname, MVMuint64 cname_length) {
    char *out_str = NULL;
    MVMUnicodeNameRegistry *result = NULL;
                                   /* number + dash + property_value + NULL */
    MVMuint64 out_str_length = length_of_num(property_code) + 1 + cname_length + 1;
    if (1024 < out_str_length)
        MVM_exception_throw_adhoc(tc, "Property value or name queried is larger than allowed.");

    out_str = alloca(sizeof(char) * out_str_length);
    snprintf(out_str, out_str_length, "%"PRIi64"-%s", property_code, cname);

    HASH_FIND(hash_handle, unicode_property_values_hashes[property_code], out_str, out_str_length - 1, result);
    return result ? result->codepoint : 0;
}
MVMint32 MVM_unicode_name_to_property_value_code(MVMThreadContext *tc, MVMint64 property_code, MVMString *name) {
    if (property_code <= 0 || MVM_NUM_PROPERTY_CODES <= property_code)
        return 0;
    else {
        MVMuint64 cname_length;
        char *cname = MVM_string_ascii_encode(tc, name, &cname_length, 0);
        return unicode_cname_to_property_value_code(tc, property_code, cname, cname_length);
    }
}
MVMint32 MVM_unicode_cname_to_property_value_code(MVMThreadContext *tc, MVMint64 property_code, const char *cname, size_t cname_length) {
    if (property_code <= 0 || MVM_NUM_PROPERTY_CODES <= property_code)
        return 0;
    else
        return unicode_cname_to_property_value_code(tc, property_code, cname, cname_length);
}

/* Look up the primary composite for a pair of codepoints, if it exists.
 * Returns 0 if not. */
MVMCodepoint MVM_unicode_find_primary_composite(MVMThreadContext *tc, MVMCodepoint l, MVMCodepoint c) {
    MVMint32 lower = l & 0xFF;
    MVMint32 upper = (l >> 8) & 0xFF;
    MVMint32 plane = (l >> 16) & 0xF;
    const MVMint32 *pcs  = comp_p[plane][upper][lower];
    if (pcs) {
        MVMint32 entries = pcs[0];
        MVMint32 i;
        for (i = 1; i < entries; i += 2)
            if (pcs[i] == c)
                return pcs[i + 1];
    }
    return 0;
}

static uv_mutex_t property_hash_count_mutex;
static int property_hash_count = 0;
static uv_once_t property_hash_count_guard = UV_ONCE_INIT;

static void setup_property_mutex(void)
{
    uv_mutex_init(&property_hash_count_mutex);
}

void MVM_unicode_init(MVMThreadContext *tc)
{
    uv_once(&property_hash_count_guard, setup_property_mutex);

    uv_mutex_lock(&property_hash_count_mutex);
    if (property_hash_count == 0) {
        generate_unicode_property_values_hashes(tc);
    }
    property_hash_count++;
    uv_mutex_unlock(&property_hash_count_mutex);
}

void MVM_unicode_release(MVMThreadContext *tc)
{
    uv_mutex_lock(&property_hash_count_mutex);
    property_hash_count--;
    if (property_hash_count == 0) {
        int i;

        for (i = 0; i < MVM_NUM_PROPERTY_CODES; i++) {
            MVMUnicodeNameRegistry *entry = NULL;
            MVMUnicodeNameRegistry *tmp   = NULL;
            unsigned bucket_tmp;
            int j;

            if (!unicode_property_values_hashes[i]) {
                continue;
            }

            for(j = i + 1; j < MVM_NUM_PROPERTY_CODES; j++) {
                if (unicode_property_values_hashes[i] == unicode_property_values_hashes[j]) {
                    unicode_property_values_hashes[j] = NULL;
                }
            }

            HASH_ITER(hash_handle, unicode_property_values_hashes[i], entry, tmp, bucket_tmp) {
                HASH_DELETE(hash_handle, unicode_property_values_hashes[i], entry);
                MVM_free(entry);
            }
            assert(!unicode_property_values_hashes[i]);
        }

        MVM_free(unicode_property_values_hashes);

        unicode_property_values_hashes = NULL;
    }
    uv_mutex_unlock(&property_hash_count_mutex);
}
/* Looks up a codepoint sequence or codepoint by name (case insensitive).
 First tries to look it up by codepoint with MVM_unicode_lookup_by_name and if
 not found as a named codepoint, lazily constructs a hash of the codepoint
 sequences and looks up the sequence name */
MVMString * MVM_unicode_string_from_name(MVMThreadContext *tc, MVMString *name) {
    MVMString * name_uc = MVM_string_uc(tc, name);
    char * cname = NULL;
    MVMUnicodeGraphemeNameRegistry *result;

    MVMGrapheme32 result_graph = MVM_unicode_lookup_by_name(tc, name_uc);
    /* If it's just a codepoint, return that */
    if (0 <= result_graph) {
        return MVM_string_chr(tc, result_graph);
    }
    /* Otherwise look up the sequence */
    else {
        const MVMint32 *uni_seq = NULL;
        cname = MVM_string_utf8_encode_C_string(tc, name_uc);
        if (!property_codes_by_seq_names) {
            generate_property_codes_by_seq_names(tc);
        }
        HASH_FIND(hash_handle, property_codes_by_seq_names, cname, strlen((const char *)cname), result);
        MVM_free(cname);
        /* If we can't find a result return an empty string */
        if (!result)
            return tc->instance->str_consts.empty;

        uni_seq = uni_seq_enum[result->structindex];
        /* The first element is the number of codepoints in the sequence */
        return MVM_unicode_codepoints_c_array_to_nfg_string(tc, (MVMCodepoint *) uni_seq + 1, uni_seq[0]);
    }

}
