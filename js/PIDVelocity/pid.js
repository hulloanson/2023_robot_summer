// copy from PID_v1.h START
const AUTOMATIC = 1
const MANUAL = 0
const DIRECT = 0
const REVERSE = 1
const P_ON_M = 0
const P_ON_E = 1
// copy from PID_v1.h END

class PID {
  constructor(State, Kp, Ki, Kd, POn, ControllerDirection) {
    // copy from PID_v1.h START for initilization
    this.dispKp = 0 // * we'll hold on to the tuning parameters in user-entered
    this.dispKi = 0 //   format for display purposes
    this.dispKd = 0 //

    this.kp = 0 // * (P)roportional Tuning Parameter
    this.ki = 0 // * (I)ntegral Tuning Parameter
    this.kd = 0 // * (D)erivative Tuning Parameter

    this.controllerDirection = 0
    this.pOn = 0

    this.lastTime = 0
    this.outputSum = 0
    this.lastInput = 0

    this.SampleTime = 0
    this.outMin = 0
    this.outMax = 0
    this.pOnE = false
    this.inAuto = false
    // copy from PID_v1.h END for initilization

    // copy from PID_v1.cpp constructor
    this.myState = State
    this.inAuto = true

    this.SetOutputLimits(0, 255) //default output limit corresponds to

    this.SampleTime = 100 //default Controller Sample Time is 0.1 seconds

    this.SetControllerDirection(ControllerDirection)
    this.SetTunings(Kp, Ki, Kd, POn)

    this.lastTime = millis() - this.SampleTime

    // my stuff
    this.rstart = millis()
    this.riteration = []
  }

  Compute() {
    if (!this.inAuto) return false
    var now = millis()

    var timeChange = now - this.lastTime
    if (timeChange >= this.SampleTime) {
      // Compute all the working error variables
      var input = this.myState.Input
      var error = this.myState.SetPoint - input

      var dInput = input - this.lastInput
      this.outputSum += this.ki * error

      // Add Proportional on Measurement, if P_ON_M is specified
      if (!this.pOnE) this.outputSum -= this.kp * dInput

      if (this.outputSum > this.outMax) this.outputSum = this.outMax
      else if (this.outputSum < this.outMin) this.outputSum = this.outMin

      // Add Proportional on Error, if P_ON_E is specified
      var output
      if (this.pOnE) output = this.kp * error
      else output = 0

      // Compute Rest of PID Output
      output += this.outputSum - this.kd * dInput

      if (output > this.outMax) output = this.outMax
      else if (output < this.outMin) output = this.outMin
      this.myState.Output = output

      // Remember some variables for next time
      this.lastInput = input
      this.lastTime = now

      // my stuff
      const rdt = now - this.rstart
      this.riteration.push({
        t: rdt,
        input: this.myState.Input,
        output: this.myState.Output,
        setPoint: this.myState.SetPoint,
        errSum: this.outputSum,
        outMax: this.outMax,
        outMin: this.outMin,
      })
      return true
    } else return false
  }

  SetTunings(Kp, Ki, Kd, POn) {
    if (Kp < 0 || Ki < 0 || Kd < 0) return

    this.pOn = POn
    this.pOnE = POn == P_ON_E

    this.dispKp = Kp
    this.dispKi = Ki
    this.dispKd = Kd

    const SampleTimeInSec = this.SampleTime / 1000.0

    this.kp = Kp
    this.ki = Ki * SampleTimeInSec
    this.kd = Kd / SampleTimeInSec

    if (this.controllerDirection == REVERSE) {
      this.kp = 0 - kp
      this.ki = 0 - ki
      this.kd = 0 - kd
    }
  }

  SetOutputLimits(Min, Max) {
    if (Min >= Max) return
    this.outMin = Min
    this.outMax = Max

    if (this.inAuto) {
      if (this.myState.Output > this.outMax) this.myState.Output = this.outMax
      else if (this.myState.Output < this.outMin)
        this.myState.Output = this.outMin

      if (this.outputSum > this.outMax) this.outputSum = this.outMax
      else if (this.outputSum < this.outMin) this.outputSum = this.outMin
    }
  }

  SetMode(Mode) {
    const newAuto = Mode == AUTOMATIC
    if (newAuto && !this.inAuto) {
      /*we just went from manual to auto*/
      Initialize()
    }
    this.inAuto = newAuto
  }

  Initialize() {
    this.outputSum = this.myState.Output
    this.lastInput = this.myState.Input
    if (this.outputSum > this.outMax) this.outputSum = this.outMax
    else if (this.outputSum < this.outMin) this.outputSum = this.outMin
  }

  SetControllerDirection(Direction) {
    if (this.inAuto && Direction != this.controllerDirection) {
      this.kp = 0 - this.kp
      this.ki = 0 - this.ki
      this.kd = 0 - this.kd
    }
    this.controllerDirection = Direction
  }

  GetKp() {
    return this.dispKp
  }
  GetKi() {
    return this.dispKi
  }
  GetKd() {
    return this.dispKd
  }
  GetMode() {
    return this.inAuto ? AUTOMATIC : MANUAL
  }
  GetDirection() {
    return this.controllerDirection
  }

  setNewSetPoint(newSetPoint) {
    this.myState.SetPoint = newSetPoint
    this.outputSum = 0
  }

  getIterationData() {
    return this.riteration
  }
}
