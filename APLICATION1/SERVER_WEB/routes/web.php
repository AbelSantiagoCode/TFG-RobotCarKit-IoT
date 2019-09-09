<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/
use Illuminate\Http\Request;

Route::get('/', function () {
    return view('welcome');
});



Route::get('/programa1', "Programa1Controller@index")->name("programa1");
Route::get('/programa1/show', "Programa1Controller@show")->name("programa1.show");
Route::get('/programa1/forwardPID', "Programa1Controller@forwardPID")->name("programa1.forwardPID");
Route::get('/programa1/backwardPID', "Programa1Controller@backwardPID")->name("programa1.backwardPID");
Route::get('/programa1/stop', "Programa1Controller@stop")->name("programa1.stop");
Route::get('/programa1/rightRotation', "Programa1Controller@rightRotation")->name("programa1.rightRotation");
Route::get('/programa1/leftRotation', "Programa1Controller@leftRotation")->name("programa1.leftRotation");
Route::get('/programa1/powerRotation', "Programa1Controller@powerRotation")->name("programa1.powerRotation");
Route::get('/programa1/moveCMForward', "Programa1Controller@moveCMForward")->name("programa1.moveCMForward");
Route::get('/programa1/moveCMBackward', "Programa1Controller@moveCMBackward")->name("programa1.moveCMBackward");
Route::get('/programa1/leftCarRotation', "Programa1Controller@leftCarRotation")->name("programa1.leftCarRotation");
Route::get('/programa1/rightCarRotation', "Programa1Controller@rightCarRotation")->name("programa1.rightCarRotation");
Route::get('/programa1/exit', "Programa1Controller@exit")->name("programa1.exit");

//Route::get('/programa2', "SpeedController@index")->name("programa2");
//Route::get('/programa3', "SpeedController@index")->name("programa3");

Route::get('/programa3', "Programa3Controller@index")->name("programa3");
Route::get('/programa3/show', "Programa3Controller@show")->name("programa3.show");
Route::get('/programa3/start', "Programa3Controller@start")->name("programa3.start");
Route::get('/programa3/stop', "Programa3Controller@stop")->name("programa3.stop");
Route::get('/programa3/exit', "Programa3Controller@exit")->name("programa3.exit");



Route::get('/programa4', "Programa4Controller@index")->name("programa4");
Route::get('/programa4/show', "Programa4Controller@show")->name("programa4.show");
Route::get('/programa4/start', "Programa4Controller@start")->name("programa4.start");
Route::get('/programa4/stop', "Programa4Controller@stop")->name("programa4.stop");
Route::get('/programa4/exit', "Programa4Controller@exit")->name("programa4.exit");
