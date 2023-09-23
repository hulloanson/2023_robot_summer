const ROBOT_IP = '192.168.86.58'

// Variables to store the key states
const keyUpDown = {
  ArrowLeft: false,
  ArrowUp: false,
  ArrowRight: false,
  ArrowDown: false,
  KeyM: false,
}

var leftInput
var rightInput

function sendCommand(p1, p2) {
  if (p2 == null) p2 = p1
  const cmd = `S${p1},${p2}`
  console.log('Command', cmd)
  ws.send(cmd)
}

function handleSendClick() {
  if (!isWSConnected()) return
  const speedLeft = parseInt(leftInput.value)
  const speedRight = parseInt(rightInput.value)
  sendCommand(speedLeft, speedRight)
}

function addFiveClick() {
  leftInput.value = parseInt(leftInput.value) + 10
  rightInput.value = parseInt(rightInput.value) + 10
  handleSendClick()
}

function subFiveClick() {
  leftInput.value = parseInt(leftInput.value) - 10
  rightInput.value = parseInt(rightInput.value) - 10
  handleSendClick()
}

function handleTurnAroundClick() {
  leftInput.value = parseInt(leftInput.value) * -1
  rightInput.value = parseInt(rightInput.value) * -1
  handleSendClick()
}

function handleStopClick() {
  leftInput.value = 0
  rightInput.value = 0
  handleSendClick()
}

const isWSConnected = function () {
  return ws && ws.readyState === ws.OPEN
}

var connected = false
const ws = new WebSocket(`ws://${ROBOT_IP}:9090`)

// Display the key states in HTML
document.addEventListener('DOMContentLoaded', function () {
  document.getElementById('robot-ip').innerText = 'Robot IP: ' + ROBOT_IP
  leftInput = document.getElementById('speed-left')
  rightInput = document.getElementById('speed-right')

  ws.onopen = function () {
    connect = true
    console.log('Connected')
  }

  ws.onclose = function () {
    connect = false
    console.log('Disconnected')
  }

  const robotStatsDisplays = ((codes, labels, ids) => {
    const r = []
    for (var i = 0; i < codes.length; i++) {
      r[codes[i]] = {
        label: labels[i],
        htmlElem: document.getElementById(ids[i] + '-display'),
      }
    }
    return r
  })(
    ['E', 'P', 'S', 'R'],
    [
      'Encoder Speed',
      'Encoder Position',
      'Target Position',
      'Target/Encoder Speed',
    ],
    ['encoder-speeds', 'positions', 'target-speeds', 'ratios']
  )

  const messageDisplay = document.getElementById('message-display')
  function numToString(reading) {
    if (Number.isInteger(reading)) return parseInt(reading)

    const num = parseFloat(reading)
    return isNaN(num) ? 'Unknown' : reading
  }

  const handleRobotMessage = function (message) {
    const [type, data] = message.split(':')
    const displayByType = robotStatsDisplays[type]
    if (displayByType != null) {
      const values = data.split(',')
      displayByType.htmlElem.innerHTML =
        displayByType.label +
        ' Left: ' +
        numToString(values[0]) +
        '<br/>' +
        displayByType.label +
        ' Right: ' +
        numToString(values[1])
    } else {
      const children = messageDisplay.children
      for (var i = children.length - 1; i > 20; i--) {
        messageDisplay.removeChild(children[i])
      }
      const p = document.createElement('p')
      p.innerHTML = message
      messageDisplay.insertBefore(p, messageDisplay.firstChild)
    }

    if (type === 'R' && data != null && data.length > 3) {
      const values = data.split(',').map(parseFloat)
      if (
        values != null &&
        values.length == 2 &&
        !isNaN(values[0]) &&
        !isNaN(values[1])
      )
        handleRobotMessage(`Left/Right Ratio: ${values[0] / values[1]}`)
    }
  }

  ws.onmessage = function (event) {
    const message = event.data
    if (message != null) handleRobotMessage(message)
  }

  // Event listeners
  document.addEventListener('keydown', function (event) {
    for (const key in keyUpDown) keyUpDown[key] = false
    keyUpDown[event.key] = keyUpDown[event.key] != null
  })

  const keyStateElems = document.querySelectorAll('.key-state')
  const connectionStateElem = document.getElementById('connection-state')
  const updateDisplay = function () {
    // for (var i = 0; i < keyStateElems.length; i++) {
    //   var div = keyStateElems[i]
    //   var span = div.querySelector('span')
    //   var key = span.id
    //   span.innerHTML = keyUpDown[key]
    // }
    connectionStateElem.innerHTML = isWSConnected()
      ? 'Connected'
      : 'Disconnected. Wait, or connect robot wifi and refresh the page.'
  }

  setInterval(() => {
    // handleSendClick()
    updateDisplay()
  }, 50)
})
