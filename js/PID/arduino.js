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
  myPID = new PID(model, 0.8, 0.5, 0.1, 0)
  myPID.SetOutputLimits(-300, 500)
  myPID2 = new PID(model2, 0.1, 0.2, 0.1, 0)
  myPID2.SetOutputLimits(-1000, 1000)

  model.SetPoint = 1000
  model.Input = d.motor.analogRead()
  model2.SetPoint = 1000
  model2.Input = d.motor2.analogRead()
  d.motor.analogWrite(0)
  d.motor.setCounter(0)
  d.motor2.analogWrite(200)
  d.motor2.setCounter(0)
}

function arduinoLoop() {
  model.Input = d.motor.analogRead()
  myPID.Compute()
  d.motor.analogWrite(model.Output)
  console.log(
    'M1',
    'I=' + Math.floor(model.Input),
    'O=' + Math.floor(model.Output)
  )

  model2.Input = d.motor2.analogRead()
  myPID2.Compute()
  d.motor2.analogWrite(model2.Output)
}
