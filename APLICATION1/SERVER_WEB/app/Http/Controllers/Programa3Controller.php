<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;


use App\Position;

class Programa3Controller extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
      // call to sendServerPipe of ApiController
      $data = "PRG3";
      \App\Http\Controllers\ApiController::sendServerPipe($data);
      $positions = Position::all();
      return view('programa3.chart')->with('positions',$positions);

    }

    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function show()
    {
      $positions = Position::all();
      return view('programa3.chart')->with('positions',$positions);
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
      $data = "START";
      \App\Http\Controllers\ApiController::sendServerPipe($data);

      // //Return to chart view
      return redirect()->route('programa3.show');
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

      // //Return to chart view
      return redirect()->route('programa3.show');
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
    /**
     * Algorithm of Program 3  to find out the next coordinates.
     *
     * @return it will be specified
     */
    public function programa3($degrees, $distance)
    {

      $lastPositon = DB::table('positions')->latest()->limit(1)->get();

      $newPosition = new Position;
      //return $lastPositon[0][0];

      if ($lastPositon[0]->direction == 1) {
        if ($degrees == 0) {
          $newPosition->direction   = 3;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 2;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 1;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 4;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 6;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        }

      } elseif ($lastPositon[0]->direction == 2) {
        if ($degrees == 0) {
          $newPosition->direction   = 5;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 3;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 2;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 1;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 4;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        }
      } elseif ($lastPositon[0]->direction == 3) {
        if ($degrees == 0) {
          $newPosition->direction   = 8;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 5;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 3;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 2;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 1;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        }
      } elseif ($lastPositon[0]->direction == 4) {
        if ($degrees == 0) {
          $newPosition->direction   = 2;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 1;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 4;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 6;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 7;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        }
      } elseif ($lastPositon[0]->direction == 5) {
        if ($degrees == 0) {
          $newPosition->direction   = 7;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 8;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 5;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 3;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 2;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        }
      } elseif ($lastPositon[0]->direction == 6) {
        if ($degrees == 0) {
          $newPosition->direction   = 1;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 4;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 6;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 7;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 8;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        }
      } elseif ($lastPositon[0]->direction == 7) {
        if ($degrees == 0) {
          $newPosition->direction   = 4;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 6;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 7;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 8;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 5;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        }
      } elseif ($lastPositon[0]->direction == 8) {
        if ($degrees == 0) {
          $newPosition->direction   = 6;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX-$distance;
        } elseif ($degrees == 45) {
          $newPosition->direction   = 7;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX;
        } elseif ($degrees == 90) {
          $newPosition->direction   = 8;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY-$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 135) {
          $newPosition->direction   = 5;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        } elseif ($degrees == 180) {
          $newPosition->direction   = 3;
          $newPosition->coordenateY = $lastPositon[0]->coordenateY+$distance;
          $newPosition->coordenateX = $lastPositon[0]->coordenateX+$distance;
        }
      }
      $newPosition->save();


    }

}
