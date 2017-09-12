// ArcadeSlam1 : Retro arcade gaming on an MCU.
// This program is designed to run on the TIMSP430G2553 and was developed in the Energia Integrated Development Environment
// For construction details and update please visit http://roboslam.com/arcadeslam
// This program is relased under the GNU Public License Version 2 a copy of which should be included with
// this collection of software
// Changelog:
// July 2017: Changed game to suit the more common 176x220 displays.  The number of sprites on screen was reduced.
// July 2017: Added 4 levels to both games and code so invaders can shoot back (Thanks to David for testing)
// July 2017 : Edited energia-1.6.10E18/hardware/energia/msp430/cores/msp430/wiring.c to force speed to 8MHz
// This allows the CPU to work at much lower battery voltages.
// July 2017 : replaced digitalWrite function calls in Display.h
// that controlled the WR and CS lines with direct port writes:
// MUCH MUCH faster
// April 2017: Modified to work with the S6D1121 TFT display
// Lots more pins used as it is a parallel interface display
// Low level I/O routines and display initialization rewritten


#include "reversebits.h"

#include "Sprite.h"
#include "Display.h"
#include "Images.h"
extern Display Screen;

void SetupButtons();
int LeftPressed();
int RightPressed();
int FirePressed();
void setup()
{
  //randomSeed(analogRead(P1_0)); // initialize the random number generator
  SetupButtons();
  Screen.begin();
}


uint8_t GameStarted = 0;
void loop()
{
  GameStarted = 0;
  Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  Screen.putText("Bread", 5, SCREEN_WIDTH/3, 50, RGBToWord(0xff, 0xff, 0x0), 0);
  Screen.putText("Board", 5, SCREEN_WIDTH/3, 70, RGBToWord(0xff, 0xff, 0x0), 0);
  Screen.putText("Games!", 5, SCREEN_WIDTH/3, 90, RGBToWord(0xff, 0xff, 0x0), 0);
  Screen.putText("Left for Brici", 14, 5, 120, RGBToWord(0xff, 0x3f, 0x3f), 0);
  Screen.putText("Right for Galaga", 16, 5, 140, RGBToWord(0x1f, 0xff, 0x1f), 0);
  Screen.putText("web:ioprog.com/bbg", 18, 5, 200, RGBToWord(0xff, 0xff, 0xff), 0);
  
 
  while (GameStarted == 0)
  {
    if (LeftPressed())
    {
      GameStarted = 1;
      PlayBrici();
    }
    if (RightPressed())
    {
      GameStarted = 1;
      PlayInvaders();
    }
    random(100); // cycle the random number generator
  }
}
void PlayBrici()
{
#define MAX_BRICI_LEVELS 4
  int Level = MAX_BRICI_LEVELS;
  int LevelComplete = 0;
  int8_t BallCount = 5;
  uint8_t Index;
  int8_t BallXVelocity = 1;
  int8_t BallYVelocity = 1;
  // This game is pushing the limits of memory.  It seems to be about 1 or 2 bytes short of blowing the stack up. :o)
#define BW 24
#define BH 10
#define SCREEN220X176
#ifdef SCREEN320X240
  #define BLOCKCOUNT 30
  Sprite Blocks[BLOCKCOUNT] = {
    { YELLBLK, 0, 50, BW, BH }, { YELLBLK, 24, 50, BW, BH }, { YELLBLK, 48, 50, BW, BH }, { YELLBLK, 72, 50, BW, BH }, { YELLBLK, 96, 50, BW, BH }, { YELLBLK, 120, 50, BW, BH }, { YELLBLK, 144, 50, BW, BH }, { YELLBLK, 168, 50, BW, BH }, { YELLBLK, 192, 50, BW, BH }, { YELLBLK, 216, 50, 24, BH },
    { GRENBLK, 0, 60, BW, BH }, { GRENBLK, 24, 60, BW, BH }, { GRENBLK, 48, 60, BW, BH }, { GRENBLK, 72, 60, BW, BH }, { GRENBLK, 96, 60, BW, BH }, { GRENBLK, 120, 60, BW, BH }, { GRENBLK, 144, 60, BW, BH }, { GRENBLK, 168, 60, BW, BH }, { GRENBLK, 192, 60, BW, BH }, { GRENBLK, 216, 60, 24, BH },
    { BLUEBLK, 0, 70, BW, BH }, { BLUEBLK, 24, 70, BW, BH }, { BLUEBLK, 48, 70, BW, BH }, { BLUEBLK, 72, 70, BW, BH }, { BLUEBLK, 96, 70, BW, BH}, { BLUEBLK, 120, 70, BW, BH }, { BLUEBLK, 144, 70, BW, BH }, { BLUEBLK, 168, 70, BW, BH }, { BLUEBLK, 192, 70, BW, BH }, { BLUEBLK, 216, 70, 24, BH }
  };
#endif
#ifdef SCREEN220X176
#define BLOCKCOUNT 21
  Sprite Blocks[BLOCKCOUNT] = {
    { YELLBLK, 3+0, 20, BW, BH }, { YELLBLK, 3+24, 20, BW, BH }, { YELLBLK, 3+48, 20, BW, BH }, { YELLBLK, 3+72, 20, BW, BH }, { YELLBLK, 3+96, 20, BW, BH }, { YELLBLK, 3+120, 20, BW, BH }, { YELLBLK, 3+144, 20, BW, BH },
    { GRENBLK, 3+0, 30, BW, BH }, { GRENBLK, 3+24, 30, BW, BH }, { GRENBLK, 3+48, 30, BW, BH }, { GRENBLK, 3+72, 30, BW, BH }, { GRENBLK, 3+96, 30, BW, BH }, { GRENBLK, 3+120, 30, BW, BH }, { GRENBLK, 3+144, 30, BW, BH },
    { BLUEBLK, 3+0, 40, BW, BH }, { BLUEBLK, 3+24, 40, BW, BH }, { BLUEBLK, 3+48, 40, BW, BH }, { BLUEBLK, 3+72, 40, BW, BH }, { BLUEBLK, 3+96, 40, BW, BH}, { BLUEBLK, 3+120, 40, BW, BH }, { BLUEBLK, 3+144, 40, BW, BH }
  };
#endif   
  Sprite Bat(BAT, 0, SCREEN_HEIGHT-20, 20, 3);
  Sprite Ball(BALL, 120, SCREEN_HEIGHT/2, 3, 3);
  // put your setup code here, to run once:
  Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  Screen.putText("Brici", 5, 10, 40, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
  delay(500);
  while (Level > 0)
  {
    Ball.move(random(10, SCREEN_WIDTH-10), SCREEN_HEIGHT/2);
    if (random(2) > 0)
      BallXVelocity = 1;
    else
      BallXVelocity = -1;
    LevelComplete = 0;
    BallYVelocity = -1;
    Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, 0);
    for (Index = BallCount; Index > 0; Index--)
      Screen.fillRectangle(SCREEN_WIDTH - Index * 15, SCREEN_HEIGHT-10, 10, 10, RGBToWord(0xff, 0xf, 0xf));
    for (Index = 0; Index < BLOCKCOUNT; Index++)
    {
      Blocks[Index].hide();
      Blocks[Index].show();
    }
    Ball.show();
    Bat.show();
    Screen.putText("Level", 5, 5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
    Screen.putNumber(MAX_BRICI_LEVELS - Level + 1, 60, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
    while (!LevelComplete)
    {
      if (RightPressed())
      {
        // Move right
        if (Bat.getX() < (SCREEN_WIDTH - Bat.getWidth()))
        {
          Bat.move(Bat.getX() + 2, Bat.getY()); // Move the bat faster than the ball
        }
      }

      if (LeftPressed())
      {
        // Move left
        if (Bat.getX() > 0)
        {
          Bat.move(Bat.getX() - 2, Bat.getY()); // Move the bat faster than the ball
        }
      }
      if (Bat.touching(Ball.getX() + Ball.getWidth() / 2, Ball.getY() + Ball.getHeight() / 2))
      {
        BallYVelocity = -BallYVelocity;
      }
      Bat.redraw(); // redraw bat as it might have lost a pixel due to collisions

      Ball.move(Ball.getX() + BallXVelocity, Ball.getY() + BallYVelocity);
      if (Ball.getX() == 2)
        BallXVelocity = -BallXVelocity;
      if (Ball.getX() == SCREEN_WIDTH - 2)
        BallXVelocity = -BallXVelocity;
      if (Ball.getY() == 2)
        BallYVelocity = -BallYVelocity;

      if (Ball.getY() >= Bat.getY() + Bat.getHeight() + 2)
      {
        BallCount--;
        if (BallCount == 0)
        {
          Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
          Screen.putText("GAME OVER", 9, 40, 100, RGBToWord(0xff, 0xff, 0xff), 0);
          Screen.putText("Fire to restart", 15, 8, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0));
          while (!FirePressed());
          return;
        }
        if (random(100) & BIT0)
          BallXVelocity = 1;
        else
          BallXVelocity = -1;

        BallYVelocity = -1;
        Ball.move(random(10, SCREEN_WIDTH - 10), random(90, 120));
        Screen.fillRectangle(SCREEN_WIDTH-5*15, SCREEN_HEIGHT-10, 120, 10, 0);
        for (Index = BallCount; Index > 0; Index--)
          Screen.fillRectangle(SCREEN_WIDTH - Index * 15, SCREEN_HEIGHT-10, 10, 10, RGBToWord(0xff, 0xf, 0xf));


      }
      LevelComplete = 1;
      for (Index = 0; Index < BLOCKCOUNT; Index++)
      {
        if (Blocks[Index].visible()) // any blocks left?
          LevelComplete = 0;
        int touch = Blocks[Index].touching(Ball.getX(), Ball.getY());
        if (touch)
        {
          Blocks[Index].hide();
          if ( (touch == 1) || (touch == 3) )
            BallYVelocity = -BallYVelocity;
          if ( (touch == 2) || (touch == 4) )
            BallXVelocity = -BallXVelocity;
        }
      }
      // No Blocks left, Move to next level.
      if ((LevelComplete == 1) && (Level > 0))
      {
        Level--;
        Screen.putText("Level", 5, 5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
        Screen.putNumber(MAX_BRICI_LEVELS - Level + 1, 60, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));

      }
      delay(Level+1); // Slow the game to human speed
    }
  }
  Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, RGBToWord(0, 0, 0xff));
  Screen.putText("VICTORY!", 8, 40, 100, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  Screen.putText("Fire to restart", 15, 8, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  while (!FirePressed());
  return;
}
void PlayInvaders()
{
  // Variables for the Invaders game
#define MAX_INVADERS 10
#define MAX_INVADER_LEVELS 4
  Sprite  Invaders[MAX_INVADERS] = {
    { InvaderImage, 3, 3, 10, 12   },
    { InvaderImage, 23, 3, 10, 12  },
    { InvaderImage, 43, 3, 10, 12  },
    { InvaderImage, 63, 3, 10, 12  },
    { InvaderImage, 83, 3, 10, 12  },
    { InvaderImage, 103, 3, 10, 12  },
    { InvaderImage, 123, 3, 10, 12 },
    { InvaderImage, 143, 3, 10, 12 },
    { InvaderImage, 163, 3, 10, 12 },
    { InvaderImage, 183, 3, 10, 12 }
  };
  Sprite  Defender(DefenderImage, SCREEN_WIDTH/2, SCREEN_HEIGHT-30, 10, 6);
#define MAX_DEFENDER_MISSILES 5
  Sprite DefenderMissiles[MAX_DEFENDER_MISSILES] = {
    { DefenderMissileImage, 0, 0, 5, 8  },
    { DefenderMissileImage, 0, 0, 5, 8  },
    { DefenderMissileImage, 0, 0, 5, 8  },
    { DefenderMissileImage, 0, 0, 5, 8  },
    { DefenderMissileImage, 0, 0, 5, 8  },
  };
#define MAX_ATTACKER_MISSILES (MAX_INVADERS)
  Sprite AttackerMissiles[MAX_ATTACKER_MISSILES] = {
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  },
    { AttackerMissileImage, 0, 0, 5, 8  }
  };
  uint8_t GameOver = 0;
  uint8_t DefenderMissileCounter = 0;
  uint8_t AttackerMissileCounter = 0;
  uint8_t InvaderCounter = 0;
  int XDirectionChange = 0;
  int YDirectionChange = 0;
  int LevelComplete = 0;
  int Level = MAX_INVADER_LEVELS;
  uint16_t x_step = 2;
  uint16_t y_step = 1;
  uint8_t ActiveAttackerMissiles = 0;

  // Initialization for the Invaders game
  int Index;
  int Toggle = 1;

  Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, 0);
  Screen.putText("MCU Galaga", 10, 10, 40, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
  Screen.putText("Only 5 missiles", 15, 10, 60, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
  Screen.putText("can be in flight", 16, 10, 80, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
  Screen.putText("at one time", 11, 10, 100, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
  delay(1000);
  while (Level > 0)
  {



    LevelComplete = 0;
    GameOver = 0;
    Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    for (Index = 0; Index < MAX_INVADERS-Level; Index++)
    {
      Invaders[Index].show();
    }     
    // Position the invaders at the top of the screen
    Invaders[0].move(0, 3);
    Invaders[1].move(15, 3);
    Invaders[2].move(30, 3);
    Invaders[3].move(45, 3);
    Invaders[4].move(60, 3);
    Invaders[5].move(75, 3);
    Invaders[6].move(90, 3);
    Invaders[7].move(105, 3);
    Invaders[8].move(120, 3);
    Invaders[9].move(135, 3);
    for (Index = 0; Index < MAX_ATTACKER_MISSILES;Index++)
      AttackerMissiles[Index].hide();    
    for (Index = 0; Index < MAX_DEFENDER_MISSILES;Index++)
      DefenderMissiles[Index].hide();          
    Screen.putText("Level", 5, 5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
    Screen.putNumber(MAX_INVADER_LEVELS - Level + 1, 60, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
    GameOver = 0;
    DefenderMissileCounter = 0;
    InvaderCounter = 0;
    XDirectionChange = 0;
    LevelComplete = 0;
    x_step = 2;
    SetupButtons();
    Defender.show();
    while (!LevelComplete )
    {
      // put your main code here, to run repeatedly:
      // Show the defender

      Defender.redraw();
      if (RightPressed())
      {
        // Move right
        if (Defender.getX() < (SCREEN_WIDTH - 11))
        {
          Defender.move(Defender.getX() + 2, Defender.getY());
        }
      }

      if (LeftPressed())
      {
        // Move left
        if (Defender.getX() > 1)
        {
          Defender.move(Defender.getX() - 2, Defender.getY());
        }
      }
      if (FirePressed() )
      {
        // User pressed Fire!
        // See if there is an empty missile-in-flight slot
        for (DefenderMissileCounter = 0; DefenderMissileCounter < MAX_DEFENDER_MISSILES; DefenderMissileCounter++)
        {
          if (DefenderMissiles[DefenderMissileCounter].visible() == 0)
          {
            // Found an empty slot, place a missile just above the centre of the defender
            DefenderMissiles[DefenderMissileCounter].show();
            DefenderMissiles[DefenderMissileCounter].move(Defender.getX() + Defender.getWidth() / 2 - DefenderMissiles[DefenderMissileCounter].getWidth() / 2, Defender.getY() - DefenderMissiles[DefenderMissileCounter].getHeight() / 2);
            break; // exit remaining for loop cycles
          }
        }
      }
      // Update defender missiles
      for (DefenderMissileCounter = 0; DefenderMissileCounter < MAX_DEFENDER_MISSILES; DefenderMissileCounter++)
      {
        if ((DefenderMissiles[DefenderMissileCounter].getY() > 0) && (DefenderMissiles[DefenderMissileCounter].visible()))
        {
          DefenderMissiles[DefenderMissileCounter].move(DefenderMissiles[DefenderMissileCounter].getX(), DefenderMissiles[DefenderMissileCounter].getY() - 2);
          // Did any missile hit an invader?
          for (InvaderCounter = 0; InvaderCounter < MAX_INVADERS; InvaderCounter++)
          {
            if (Invaders[InvaderCounter].within(DefenderMissiles[DefenderMissileCounter].getX(), DefenderMissiles[DefenderMissileCounter].getY()))
            {
              // Missile meets invader!
              Invaders[InvaderCounter].hide();
              DefenderMissiles[DefenderMissileCounter].hide();
              // Do an explosion at that location
              Explode(DefenderMissiles[DefenderMissileCounter].getX(), DefenderMissiles[DefenderMissileCounter].getY());
            }
          }
        }
        else
        {
          if (DefenderMissiles[DefenderMissileCounter].visible())
            DefenderMissiles[DefenderMissileCounter].hide();
        }

      }

      // Update attacker missiles
      ActiveAttackerMissiles = MAX_ATTACKER_MISSILES;
      for (AttackerMissileCounter = 0; AttackerMissileCounter < MAX_ATTACKER_MISSILES; AttackerMissileCounter++)
      {
        if ((AttackerMissiles[AttackerMissileCounter].getY() < SCREEN_HEIGHT-20) && (AttackerMissiles[AttackerMissileCounter].visible()))
        {
          AttackerMissiles[AttackerMissileCounter].move(AttackerMissiles[AttackerMissileCounter].getX(), AttackerMissiles[AttackerMissileCounter].getY() + 2);

          // Did any attacker missile hit the defender?
          if (Defender.within(AttackerMissiles[AttackerMissileCounter].getX() + AttackerMissiles[AttackerMissileCounter].getWidth() / 2, AttackerMissiles[AttackerMissileCounter].getY() + AttackerMissiles[AttackerMissileCounter].getHeight() / 2))
          {
            Explode(Defender.getX(), Defender.getY());
            Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            Screen.putText("GAME OVER", 9, 40, 100, RGBToWord(0xff, 0xff, 0xff), 0);
            Screen.putText("Fire to restart", 15, 8, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0));
            while (!FirePressed());
            return;
          }

        }
        else
        {
          if (AttackerMissiles[AttackerMissileCounter].visible())
            AttackerMissiles[AttackerMissileCounter].hide();
          if (ActiveAttackerMissiles > 0)
            ActiveAttackerMissiles--;

        }
      }
      // Update invaders
      XDirectionChange = 0;
      YDirectionChange = 0;
      LevelComplete = 1;
      for (InvaderCounter = 0; InvaderCounter < MAX_INVADERS; InvaderCounter++)
      {
        if (Invaders[InvaderCounter].visible())
        {
          if (ActiveAttackerMissiles < MAX_ATTACKER_MISSILES - Level)
          {
            // Spawn a new attacker missile
            AttackerMissileCounter = 0;
            uint8_t MissileFired = 0;
            while ( (AttackerMissileCounter < MAX_ATTACKER_MISSILES) && (!MissileFired))
            {
              if ((AttackerMissiles[AttackerMissileCounter].visible() == 0) && (random(0,50)==0))
              {
                ActiveAttackerMissiles++;
                AttackerMissiles[AttackerMissileCounter].show();
                AttackerMissiles[AttackerMissileCounter].move(Invaders[InvaderCounter].getX() + Invaders[InvaderCounter].getWidth() / 2 - DefenderMissiles[DefenderMissileCounter].getWidth() / 2, Invaders[InvaderCounter].getY() + DefenderMissiles[DefenderMissileCounter].getHeight() / 2);
                MissileFired = 1;
              }
              AttackerMissileCounter++;
            }
          }
          LevelComplete = 0; // didn't win yet, there are still invaders
          Invaders[InvaderCounter].move(Invaders[InvaderCounter].getX() + x_step, Invaders[InvaderCounter].getY() + y_step);
          if (Invaders[InvaderCounter].getX() >= (SCREEN_WIDTH - Invaders[InvaderCounter].getWidth()))
            XDirectionChange = 1;
          if (Invaders[InvaderCounter].getX() == 0)
            XDirectionChange = 1;
          if (Invaders[InvaderCounter].getY() > SCREEN_HEIGHT - 100)
          {
            //Invaders[InvaderCounter].move(Invaders[InvaderCounter].getX(), 0);
            YDirectionChange = 1;

            if (Invaders[InvaderCounter].getY() < 3)
            {
              YDirectionChange = 1;
            }

          }
        }
      }
      if (XDirectionChange) // Did an invader hit either edge?
        x_step = -x_step; // if so, then reverse direction
      if (YDirectionChange) // Did an invader hit either edge?
        y_step = -y_step; // if so, then reverse direction
    } // End of while (!LevelComplete )
    if ((LevelComplete == 1) && (Level > 0))
    {
      Level--;
      Screen.putText("Level", 5, 5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
      Screen.putNumber(MAX_INVADER_LEVELS - Level + 1, 60, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));

    }
  } // end of while (Level > 0)
  Screen.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBToWord(0, 0, 0xff));
  Screen.putText("VICTORY!", 8, 40, 100, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  Screen.putText("Fire to restart", 15, 8, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  while (!FirePressed());
  return;



}
void SetupButtons()
{
  // Button layout:
  // P1_6 : Left
  // P2_7 : Fire
  // P2_6 : Right
  // Set up the buttons
  pinMode(P1_6, INPUT);
  pinMode(P2_6, INPUT);
  pinMode(P2_7, INPUT);
  // enable pull-up resistors
  P1REN |= (BIT6);
  P1OUT |= (BIT6);
  P2REN |= (BIT6 + BIT7);
  P2OUT |= (BIT6 + BIT7);
}
int LeftPressed()
{
  if ( (P1IN & BIT6) == 0)
    return 1;
  else
    return 0;
}
int RightPressed()
{
  if ( (P2IN & BIT6) == 0)
    return 1;
  else
    return 0;
}
int FirePressed()
{
  static int PreviousState = 0;
  if ((P2IN & BIT7) == 0)
  {
    if (PreviousState == 0)
    {
      PreviousState = 1;
      return 1;
    }
    else
      return 0;

  }
  else
  {
    PreviousState = 0;
    return 0;
  }
}
void Explode(uint16_t X, uint16_t Y)
{
  Sprite Bang(ExplosionImage, X - 5, Y - 6, 10, 12);
  Bang.show();
  delay(100);
  Bang.hide();
}


