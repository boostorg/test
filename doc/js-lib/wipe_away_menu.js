JS.include( 'vis_object_api.js', viso_path );
JS.include( 'scheduler_api.js', viso_path );
JS.include( 'transitions.js', viso_path );

function WipeAwayMenu( menu_id, menu_root_id, start_closed, direction, show_info, hide_info ) {
    this.menu_root     = viso_get( menu_root_id );
    this.menu          = viso_get( menu_id );
    this.trans_task_id = null;
    this.direction     = direction ? direction : "right";
    this.start_closed  = start_closed;
    this.menu_size     = viso_get_sizes( this.menu );

    switch( this.direction ) {
    case "left":
        this.show_info = show_info ? show_info : { tran:"wipe-left", speed:10 };
        this.hide_info = hide_info ? hide_info : { tran:"wipe-right", speed:10 };
        break;
    case "right":
        this.show_info = show_info ? show_info : { tran:"wipe-right", speed:10 };
        this.hide_info = hide_info ? hide_info : { tran:"wipe-left", speed:10 };
        break;
    case "top":
        this.show_info = show_info ? show_info : { tran:"wipe-up", speed:10 };
        this.hide_info = hide_info ? hide_info : { tran:"wipe-down", speed:10 };
        break;
    case "down":
        this.show_info = show_info ? show_info : { tran:"wipe-down", speed:10 };
        this.hide_info = hide_info ? hide_info : { tran:"wipe-up", speed:10 };
        break;
    }

    this.init();

    viso_add_event_handler( "resize", new Callback( WipeAwayMenu.prototype.init, this ) );
    viso_add_event_handler( "scroll", new Callback( WipeAwayMenu.prototype.hide, this ) );
}

WipeAwayMenu.prototype.init = function () {
    this.menu_state = 1; // 0 - closing 1 - closed, 2 - opening, 3 - open

    var root_pos    = viso_get_page_position( this.menu_root );
    var root_size   = viso_get_sizes( this.menu_root );

    var parent_pos, menu_pos;

    switch( this.direction ) {
    case "left":
        menu_pos = new Pair( root_pos.x - this.menu_size.x - 5,
                             root_pos.y + (root_size.y-this.menu_size.y)/2 );
        parent_pos = new Pair( menu_pos.x,
                               Math.min( root_pos.y,menu_pos.y ) );
        break;
    case "right":
        menu_pos = new Pair( root_pos.x + root_size.x + 5,
                             root_pos.y + (root_size.y-this.menu_size.y)/2 );
        parent_pos = new Pair( root_pos.x,
                               Math.min( root_pos.y,menu_pos.y ) );
        break;
    case "top":
        menu_pos = new Pair( root_pos.x + (root_size.x-this.menu_size.x)/2,
                             root_pos.y - this.menu_size.y - 5 );
        parent_pos = new Pair( Math.min( root_pos.x,menu_pos.x ),
                               menu_pos.y );
        break;
    case "down":
        menu_pos = new Pair( root_pos.x + (root_size.x-this.menu_size.x)/2,
                             root_pos.y + root_size.y + 5 );
        parent_pos = new Pair( Math.min( root_pos.x,menu_pos.x ),
                               root_pos.y );
        break;
    }
    if( !this.show_info.x_step )
        this.show_info.x_step = 0;

    if( !this.show_info.y_step )
        this.show_info.y_step = 0;

    if( this.menu.parentNode.id == "wipe-away-menu-holder" ) {
        viso_move_to( this.menu.parentNode, parent_pos );
        viso_insure_width( this.menu.parentNode, this.menu_size.x + root_size.x + 5 );
        viso_insure_height( this.menu.parentNode, Math.max( this.menu_size.y, root_size.y ) );
    }

    viso_move_to( this.menu, menu_pos );
    viso_set_displayed( this.menu, !this.start_closed );
}

WipeAwayMenu.prototype._end_show = function() { this.menu_state = 3; }
WipeAwayMenu.prototype._end_hide = function() { this.menu_state = 1; }

WipeAwayMenu.prototype.show = function () {
    if( this.menu_state > 1 )
        return;

    if( this.menu_state == 0 )
        TaskScheduler.cancel_task( this.trans_task_id );

    this.menu_state = 2;
    this.trans_task_id = viso_make_transition( null, this.menu, 
                                               this.show_info.tran, this.show_info.speed, 
                                               new Callback( WipeAwayMenu.prototype._end_show, this ),
                                               this.show_info.x_step, this.show_info.y_step, true );
}

WipeAwayMenu.prototype.hide = function () {
    if( this.menu_state < 2 )
        return;

    if( this.menu_state == 2 ) {
        TaskScheduler.cancel_task( this.trans_task_id );
    }

    this.menu_state = 0;
    this.trans_task_id = viso_make_transition( this.menu, null, 
                                               this.hide_info.tran, this.hide_info.speed, 
                                               new Callback( WipeAwayMenu.prototype._end_hide, this ), 
                                               this.show_info.x_step, this.show_info.y_step, true );
}

// EOF
