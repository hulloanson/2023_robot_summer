const ROBOT_IP = '192.168.86.52'

// Variables to store the key states
const keyUpDown = {
  ArrowLeft: false,
  ArrowUp: false,
  ArrowRight: false,
  ArrowDown: false,
}

var connected = false

// Display the key states in HTML
document.addEventListener('DOMContentLoaded', function () {
  const ws = new WebSocket(`ws://${ROBOT_IP}:9090`)

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
      'Target Speed',
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

  ws.onmessage = function (event) {
    // console.log('message', event)
    const message = event.data
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
      messageDisplay.innerHTML = message
      // const p = document.createElement('p')
      // p.innerHTML = message
      // messageDisplay.insertBefore(p, messageDisplay.firstChild)
    }
  }

  const isWSConnected = function () {
    return ws && ws.readyState === ws.OPEN
  }

  // Event listeners
  document.addEventListener('keydown', function (event) {
    // console.log("keydown", event);
    keyUpDown[event.key] = true
  })

  document.addEventListener('keyup', function (event) {
    // console.log("keyup", event);
    keyUpDown[event.key] = false
  })

  const keyStateElems = document.querySelectorAll('.key-state')
  const connectionStateElem = document.getElementById('connection-state')
  const updateDisplay = function () {
    for (var i = 0; i < keyStateElems.length; i++) {
      var div = keyStateElems[i]
      var span = div.querySelector('span')
      var key = span.id
      span.innerHTML = keyUpDown[key]
    }
    connectionStateElem.innerHTML = isWSConnected()
      ? 'Connected'
      : 'Disconnected. Wait, or connect robot wifi and refresh the page.'
  }

  const sendCommands = function () {
    if (!isWSConnected()) return
    const speed = parseInt(document.getElementById('speed-setting').value)

    const velocity = keyUpDown.ArrowDown
      ? -speed
      : keyUpDown.ArrowUp
      ? speed
      : 0
    ws.send('V' + velocity)

    // const turnAmount = keyUpDown.ArrowLeft
    //   ? -speed
    //   : keyUpDown.ArrowRight
    //   ? speed
    //   : 0
    // ws.send('T' + turnAmount)
  }

  setInterval(() => {
    sendCommands()
    updateDisplay()
  }, 50)
})
