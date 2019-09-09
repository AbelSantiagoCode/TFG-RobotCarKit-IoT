<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Speed;


class Programa1Controller extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
      // call to sendServerPipe of ApiController
      $data = "PRG1";
      \App\Http\Controllers\ApiController::sendServerPipe($data);
      return view('programa1.control');
    }

    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function show()
    {
      return view('programa1.control');
    }

    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function forwardPID(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "FORWARD_PID,".$request->forwardPID;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }


    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function backwardPID(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "BACKWARD_PID,".$request->backwardPID;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }


    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function stop()
    {
      // call to sendServerPipe of ApiController
      $data = "STOP";
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');
    }

    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function rightRotation()
    {
      // call to sendServerPipe of ApiController
      $data = "ROTATE,R";
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }

    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function leftRotation()
    {
      // call to sendServerPipe of ApiController
      $data = "ROTATE,L";
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }


    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function powerRotation(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "POWER_ROTATION,".$request->powerRotation;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }


    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function moveCMForward(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "FORWARD_CM,".$request->moveCMForward;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }

    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function moveCMBackward(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "BACKWARD_CM,".$request->moveCMBackward;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }

    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function leftCarRotation(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "ROTATE_CAR,L,".$request->leftCarRotation;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');

    }

    /**
     * Send START TO ServerPipe with a initial RPM value
     *
     *@param  RPM SetPoint for PID  $request
     *@return \Illuminate\Http\Response
     */
    public function rightCarRotation(Request $request)
    {
      // call to sendServerPipe of ApiController
      $data = "ROTATE_CAR,R,".$request->rightCarRotation;
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      //Return to chart view
      return redirect()->route('programa1.show');
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
