include ( 'callback_api.js', viso_path );

function ScheduledTaskInfo( func, once, finish_callback_func ) {
    this.func    = func;
    this.tid     = null;
    this.once    = once ? true : false;
    this.counter = 0;
    this.cb      = finish_callback_func;
}

function Scheduler() {
   this.uniq_task_id = 0;
   this.tasks_info   = new Array();
}

TaskScheduler = new Scheduler();

Scheduler.prototype.schedule = function( func, timeout, once, end_callback ) {
    var task_id   = "task" + this.uniq_task_id++;
    var task_info = new ScheduledTaskInfo( func, once, end_callback );

    task_info.tid = setInterval( 'TaskScheduler.run("' + task_id + '")', timeout );

    this.tasks_info[task_id] = task_info;

    return task_id;
}

Scheduler.prototype.cancel_task = function( task_id ) {
    var task_info = this.tasks_info[task_id];

    clearInterval( task_info.tid );

    if( task_info.cb )
        task_info.cb.run();

    delete this.tasks_info[task_id];
}

Scheduler.prototype.run = function( task_id ) {
    var task_info = this.tasks_info[task_id];

    if( !task_info.func.run( task_info.counter++ ) || task_info.once )
        this.cancel_task( task_id );
}

// EOF
