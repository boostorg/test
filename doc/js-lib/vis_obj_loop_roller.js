include ( 'vis_object_api.js', viso_path );
include ( 'transitions.js', viso_path );

function VisObjLoopRoller( transition_id ) {
    this.curr_index    = 0;
    this.objs          = new Array();
    this.transition_id = transition_id;
    var objs_ids_start = 1;

    if( transition_id ) {
        this.speed = arguments[1];
        this.step = arguments[2];
        objs_ids_start = 3;
    }

    for( var i = objs_ids_start; i < arguments.length; i++ ) {
        var obj = viso_get( arguments[i] );
        this.objs[i-objs_ids_start] = viso_get( arguments[i] );
    }
}


VisObjLoopRoller.prototype.roll = function() {
    var from_obj = this.objs[this.curr_index++];

    this.curr_index %= this.objs.length;
    var to_obj = this.objs[this.curr_index];

    if( !this.transition_id ) {
        var was_displayed = viso_is_displayed( from_obj );
        viso_set_displayed( from_obj, viso_is_displayed( to_obj ) );
        viso_set_displayed( to_obj, was_displayed );
    }
    else {
        viso_make_transition( from_obj, to_obj, this.transition_id, this.speed, null, this.step );
    }
}

// EOF
