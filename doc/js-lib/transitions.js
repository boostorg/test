include ( 'callback_api.js', viso_path );
include ( 'scheduler_api.js', viso_path );
include ( 'vis_object_api.js', viso_path );

function viso_make_transition( from_vis_obj, to_vis_obj, transition, speed, 
                               callback, x_step_size, y_step_size, 
                               dont_align_parent ) {
    var dcl_from = new Quadruple;
    var dcl_to   = new Quadruple;
    var total_steps;

    x_step_size = x_step_size ? x_step_size : 10;
    x_step_size = Math.max( x_step_size, 2 );

    y_step_size = y_step_size ? y_step_size : 2;
    y_step_size = Math.max( y_step_size, 2 );

    viso_clip( to_vis_obj, new Quadruple );

    viso_display( to_vis_obj );

    var from_sizes = from_vis_obj ? viso_get_sizes( from_vis_obj ) : new Pair();
    var to_sizes   = to_vis_obj ? viso_get_sizes( to_vis_obj ) : new Pair();

    var x_work_size = Math.max( from_sizes.x, to_sizes.x );
    var y_work_size = Math.max( from_sizes.y, to_sizes.y );

    viso_clip( from_vis_obj, 
               new Quadruple( 0, 0, x_work_size, y_work_size ) );

    switch( transition ) {
    case "wipe-right":
        viso_clip( to_vis_obj,
                   new Quadruple( 0, 0, 0, y_work_size ) );
        dcl_from.x1 = x_step_size;
        dcl_to.x3   = x_step_size;
        total_steps = x_work_size / x_step_size;
        break;
    case "wipe-left":
        viso_clip( to_vis_obj,
                   new Quadruple( x_work_size, 0, x_work_size, y_work_size ) );
        dcl_from.x3 = -x_step_size;
        dcl_to.x1   = -x_step_size;
        total_steps = x_work_size / x_step_size;
        break;
    case "wipe-down":
        viso_clip( to_vis_obj,
                   new Quadruple( 0, 0, x_work_size, 0 ) );
        dcl_from.x2 = y_step_size;
        dcl_to.x4   = y_step_size;
        total_steps = y_work_size / y_step_size;
        break;
    case "wipe-up":
        viso_clip( to_vis_obj,
                   new Quadruple( 0, y_work_size, x_work_size, y_work_size ) );
        dcl_from.x4 = -y_step_size;
        dcl_to.x2   = -y_step_size;
        total_steps = y_work_size / y_step_size;
        break;
    case "split-in-vert":
        viso_set_zindex( from_vis_obj, 1 );
        viso_set_zindex( to_vis_obj, 0 );

        viso_clip( to_vis_obj,
                   new Quadruple( 0, 0, x_work_size, y_work_size ) );
        dcl_from.x1 = Math.ceil( x_step_size/2 );
        dcl_from.x3 = -Math.ceil( x_step_size/2 );
        total_steps = x_work_size/x_step_size;
        break;
    case "split-out-vert":
        viso_set_zindex( from_vis_obj, 0 );
        viso_set_zindex( to_vis_obj, 1 );

        viso_clip( to_vis_obj,
                   new Quadruple( x_work_size/2, 0, x_work_size/2, y_work_size ) );
        dcl_to.x1 = -Math.ceil( x_step_size/2 );
        dcl_to.x3 = Math.ceil( x_step_size/2 );
        total_steps = x_work_size/x_step_size;
        break;
    case "split-in-out-vert":
        viso_set_zindex( from_vis_obj, 0 );
        viso_set_zindex( to_vis_obj, 1 );

        viso_clip( to_vis_obj,
                   new Quadruple( x_work_size/2, 0, x_work_size/2, y_work_size ) );
        dcl_from.x1 = Math.ceil( x_step_size/2 );
        dcl_from.x3 = -Math.ceil( x_step_size/2 );
        dcl_to.x1 = -Math.ceil( x_step_size/2 );
        dcl_to.x3 = Math.ceil( x_step_size/2 );
        total_steps = x_work_size/x_step_size;
        break;
    case "split-in-hor":
        viso_set_zindex( from_vis_obj, 1 );
        viso_set_zindex( to_vis_obj, 0 );

        viso_clip( to_vis_obj,
                   new Quadruple( 0, 0, x_work_size, y_work_size ) );
        dcl_from.x2 = Math.ceil( y_step_size/2 );
        dcl_from.x4 = -Math.ceil( y_step_size/2 );
        total_steps = y_work_size/y_step_size;
        break;
    case "split-out-hor":
        viso_set_zindex( from_vis_obj, 0 );
        viso_set_zindex( to_vis_obj, 1 );

        viso_clip( to_vis_obj,
                   new Quadruple( 0, y_work_size/2, x_work_size, y_work_size/2 ) );
        dcl_to.x2   = -Math.ceil( y_step_size/2 );
        dcl_to.x4   = Math.ceil( y_step_size/2 );
        total_steps = y_work_size/y_step_size;
        break;
    case "split-in-out-hor":
        viso_set_zindex( from_vis_obj, 0 );
        viso_set_zindex( to_vis_obj, 1 );

        viso_clip( to_vis_obj,
                   new Quadruple( 0, y_work_size/2, x_work_size, y_work_size/2 ) );
        dcl_from.x2 = Math.ceil( y_step_size/2 );
        dcl_from.x4 = -Math.ceil( y_step_size/2 );
        dcl_to.x2   = -Math.ceil( y_step_size/2 );
        dcl_to.x4   = Math.ceil( y_step_size/2 );
        total_steps = y_work_size/y_step_size;
        break;
    case "box-in":
        viso_set_zindex( from_vis_obj, 1 );
        viso_set_zindex( to_vis_obj, 0 );

        viso_clip( to_vis_obj,
                   new Quadruple( 0, 0, x_work_size, y_work_size ) );
        total_steps = Math.max( x_work_size/x_step_size, y_work_size/y_step_size );
        x_step_size = Math.ceil( x_work_size/(2*total_steps) );
        y_step_size = Math.ceil( y_work_size/(2*total_steps) );
        dcl_from.x1 = x_step_size;
        dcl_from.x2 = y_step_size;
        dcl_from.x3 = -x_step_size;
        dcl_from.x4 = -y_step_size;
        break;
    case "box-out":
        viso_set_zindex( from_vis_obj, 0 );
        viso_set_zindex( to_vis_obj, 1 );

        viso_clip( to_vis_obj,
                   new Quadruple( x_work_size/2, y_work_size/2, x_work_size/2, y_work_size/2 ) );
        total_steps = Math.max( x_work_size/x_step_size, y_work_size/y_step_size );
        x_step_size = Math.ceil( x_work_size/(2*total_steps) );
        y_step_size = Math.ceil( y_work_size/(2*total_steps) );
        dcl_to.x1 = -x_step_size;
        dcl_to.x2 = -y_step_size;
        dcl_to.x3 = x_step_size;
        dcl_to.x4 = y_step_size;
        break;
    case "box-in-out":
        viso_set_zindex( from_vis_obj, 0 );
        viso_set_zindex( to_vis_obj, 1 );

        viso_clip( to_vis_obj,
                   new Quadruple( x_work_size/2, y_work_size/2, x_work_size/2, y_work_size/2 ) );
        total_steps = Math.max( x_work_size/x_step_size, y_work_size/y_step_size );
        x_step_size = Math.ceil( x_work_size/(2*total_steps) );
        y_step_size = Math.ceil( y_work_size/(2*total_steps) );
        dcl_from.x1 = x_step_size;
        dcl_from.x2 = y_step_size;
        dcl_from.x3 = -x_step_size;
        dcl_from.x4 = -y_step_size;
        dcl_to.x1 = -x_step_size;
        dcl_to.x2 = -y_step_size;
        dcl_to.x3 = x_step_size;
        dcl_to.x4 = y_step_size;
        break;
    default:
        return;
    }
    if( !dont_align_parent && to_vis_obj ) {
        var parent       = viso_get_parent( to_vis_obj );
        var parent_sizes = viso_get_sizes( parent );

        if( parent_sizes.y < to_sizes.y ) {
            viso_set_height( parent, to_sizes.y );
        }

        if( parent_sizes.x < to_sizes.x ) {
            viso_set_width( parent, to_sizes.x );
        }
    }

    callback = callback ? callback : null;
    var bound_work_func = new Callback( viso_do_transition, null,
                                        from_vis_obj, to_vis_obj, 
                                        dcl_from, dcl_to, 
                                        total_steps, dont_align_parent );
    return TaskScheduler.schedule( bound_work_func, speed, false, callback );
}

function viso_do_transition( from_vis_obj, to_vis_obj, 
                             dcl_from, dcl_to, total_steps,
                             dont_align_parent, counter ) {
    viso_change_clip_by( from_vis_obj, dcl_from );
    viso_change_clip_by( to_vis_obj, dcl_to );

    if( counter < total_steps )
        return true;

    viso_remove( from_vis_obj );

    if( !dont_align_parent && to_vis_obj ) {
        var parent       = viso_get_parent( to_vis_obj );
        var parent_sizes = viso_get_sizes( parent );
        var to_sizes     = viso_get_sizes( to_vis_obj );

        if( parent_sizes.y > to_sizes.y ) {
            viso_set_height( parent, to_sizes.y );
        }
        if( parent_sizes.x > to_sizes.x ) {
            viso_set_width( parent, to_sizes.x );
        }
    }

    return false;
}

// EOF
