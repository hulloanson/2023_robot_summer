const d = {
  motor: new Motor(50, 200),
  motor2: new Motor(50, 300),
}

const model = {
  Input: 0,
  Output: 0,
  SetPoint: 0,
}

const model2 = {
  Input: 0,
  Output: 0,
  SetPoint: 0,
}

var myPID
var myPID2

function arduinoSetup() {
  myPID = new PID(model, 0.5, 0.5, 0.1, 0)
  myPID.SetOutputLimits(-250, 150)
  myPID2 = new PID(model2, 0.9, 0.5, 0.1, 0)
  myPID2.SetOutputLimits(-250, 150)

  model.SetPoint = 10
  model2.SetPoint = 10
  d.motor.analogWrite(0)
  d.motor.setCounter(0)
  d.motor2.analogWrite(0)
  d.motor2.setCounter(0)
  d.motor2.velDefect = 2
}

function arduinoLoop() {
  model.Input = d.motor.analogRead(1)
  myPID.Compute()
  d.motor.analogWrite(model.Output)

  model2.Input = d.motor2.analogRead(1)
  myPID2.Compute()
  d.motor2.analogWrite(model2.Output)
}
