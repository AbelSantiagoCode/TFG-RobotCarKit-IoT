<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;
use App\Speed;
use App\Position;

class ApiController extends Controller
{

    /**
     * Recive the message from ServerPipe inside Request
     *
     */
    public function rcvServerPipe(Request $request)
    {
      $message = $request->input('message');
      $message = explode(",",$message);
      if ($message[0] == "programa3") {
        $degrees = (int)$message[1];
        $distance = (int)$message[2];
        // SAVE THE VALUES INTO THE TABLE POSITIONS WITHOUT VALIDATE
        //\App\Http\Controllers\Programa3Controller::programa3($degrees,$distance);
        $this->programa3($degrees,$distance);
        //return $degrees;
        $this->broadcastWebSocket();

      }
      else if ($message[0] == "programa4") {
        // FORMAT MESSAGE : Programa4,leftSpeed,rightSpeed
        // GET THE SPEEDS OF THE MOTORS
        $leftSpeed = (int)$message[1];
        $rightSpeed = (int)$message[2];
        // SAVE THE VALUES INTO THE TABLE SPEED WITHOUT VALIDATE
        $speed = new Speed;
        $speed->leftSpeed = $leftSpeed;
        $speed->rightSpeed = $rightSpeed;
        $speed->save();
        $this->broadcastWebSocket();
      }
    }

    /**
     * Sends the message from Client-Server Web to ServerPipe
     *
     */
    public static function sendServerPipe($message)
    {
      /* Obtener el puerto para el servicio WWW. */
      $service_port = 2000;

      /* Obtener la dirección IP para el host objetivo. */
      $address = "192.168.0.159";

      /* Crear un socket TCP/IP. */
      $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
      if ($socket === false) {
          echo "socket_create() falló: razón: " . socket_strerror(socket_last_error()) . "\n";
      }

      $result = socket_connect($socket, $address, $service_port);
      if ($result === false) {
          echo "socket_connect() falló.\nRazón: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
      }

      /* Enviar mensaje a server socket. */
      socket_write($socket, $message, strlen($message));
      socket_close($socket);

    }

    /**
    * Broadcast to clients that new data is avilable.
    *
    * @return \Illuminate\Http\Response
    */
    public function broadcastWebSocket()
    {
      $data = [
        'topic_id' => 'onAlertMessage',
        'data' => 'someData'
      ];
      \App\Socket\Pusher::sentDataToServer($data);

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
