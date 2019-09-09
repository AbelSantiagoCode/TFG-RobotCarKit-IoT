<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Speed;


class Programa4Controller extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
      // call to sendServerPipe of ApiController
      $data = "PRG4";
      \App\Http\Controllers\ApiController::sendServerPipe($data);
      $speeds = Speed::all();
      return view('programa4.chart')->with('speeds',$speeds);
    }

    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function show()
    {
      $speeds = Speed::all();
      return view('programa4.chart')->with('speeds',$speeds);
    }


    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function start(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "START,".$request->setPoint;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      // //Return to chart view
      // $speeds = Speed::all();
      // return view('programa4.chart')->with('speeds',$speeds);
      return redirect()->route('programa4.show');
    }


    /**
     * Send STOP to ServerPipe with a initial RPM value
     *
     */
    public function stop()
    {
      // call to sendServerPipe of ApiController
      $data = "STOP";
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      $speeds = Speed::all();
      return view('programa4.chart')->with('speeds',$speeds);
    }


    /**
     * Send EXIT to ServerPipe with a initial RPM value
     *
     */
    public function exit()
    {
      // call to sendServerPipe of ApiController
      $data = "EXIT";
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to welcome view
      return redirect('/');
    }


}
