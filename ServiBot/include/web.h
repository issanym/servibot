const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ServiBot</title>
    <style>
    *{
    font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
    
}
body{
    background-color: #E4E4E4;
    margin: 0px;
}
nav{
    background-color: #002861;
    font-size: 25px;
    font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
    color: white;
    padding: 10px;
}
footer{
    background-color: #002861;
    font-size: 18px;
    color: white;
    padding: 10px;
    text-align: center;
}
.update{
    
    padding:0px;
    display: flex;
    margin-left: 20px;
    /*width: 25%;*/
}
.status{
    /*background-color: antiquewhite;*/
    margin: 0px;
    padding: 20px;
    width: 60%;
}
.status-container{
    display: flex;
    padding: 0px 0px 20px 0px;
    margin-left: 0px;
}
.state{
    background-color: transparent;
    border: 1px black solid;
    border-radius: 15px;
    padding: 10px;
    font-size: 15px;
    font-weight: bold;
    margin-left: 5px;
}
.idle{
    background-color: orangered;
    color: black;
}
.moving{
    background-color: rgb(255, 217, 0);
    color: black;
}
.arrived{
    background-color: #07d054;
    color: black;
}
.returning{
    background-color: rgb(91, 234, 186);
    color: black;
}
.target{
    /*background-color: chartreuse;*/
    margin: 0px;
    width: 40%;
    padding: 20px;
}
.bar{
    background-color: #07d054;
    color: black;
    border: 1px black solid;
    border-radius: 15px;
    padding: 10px;
    font-size: 15px;
    font-weight: bold;
    margin-left: 5px;
    text-align: center;
    width: 150px;
}

/*------------------------------------------------------------*/
.container{
    /*display: flex;*/
}
.tables-container{
    background-color: burlywood;
    /*margin-right: 10px;*/
    /*width: 75%;*/
     display: flex;
    justify-content: center;
    padding-top: 50px;
}
.tables{
    padding: 5px 30px 20px 30px;
    background-color: #ddd;
    margin: 20px 10px 20px 10px;
    border-radius: 15px;
    border: 2px solid black;
    /*height: 350px;*/
    font-weight: bold;
    padding-top: 50px;
    padding-bottom: 90px;
}
.request{
    display:inline-block;
    background-color: transparent;
    border: 1px solid black;
    border-radius: 15px;
    padding: 10px;
    font-size: 15px;
    margin-left: 20px;

}
.requesting{
    display:inline-block;
    background-color: #e65613;
}
.ttl{
    margin: 0px;
    padding: 30px;
    text-align: center;
}

.g-btn{
    background-color: #07d054;
    border: 1px solid black;
    border-radius: 15px;
    padding: 10px;
    font-size: 15px;
    font-weight: bold;
}
.g-btn:hover{
    background-color: #02712cd0;
    border: 1px solid black;
    border-radius: 15px;
    color: white;
    padding: 10px;
    font-size: 15px;
}
/*-------------------------------------------------------*/
.cmd-container{
    display: flex;
    padding: 30px;
    justify-content: center;
}
.cmd-base {
    border: 1px black solid;
    border-radius: 15px;
    padding: 10px;
    font-size: 15px;
    font-weight: bold;
    margin-left: 5px;
    text-align: center;
    width: 150px;
}
.resume{
    background-color: #07d054;/*rgb(26, 233, 26);*/
}
.resume:hover{
    background-color: #02712cd0;/*rgb(26, 233, 26);*/
    color: whitesmoke;
}
.stop{
    background-color: #e65613;
}
.stop:hover{
    background-color: #8b330a;
    color: whitesmoke;
}
.return{
    background-color: rgb(91, 234, 186);
}
.return:hover{
    background-color: rgb(40, 98, 79);
    color: whitesmoke;
}
    </style>
</head>
<body>
    <nav> ServiBot </nav>
    <div class="container">
        <div class="update">
            <div class="status">
                <h2>Status</h2>
                <div  class="status-container">
                    <div id="idleID" class="state"> Idle</div>
                    <div id="movingID" class="state"> En route</div>
                    <div id="arrivedID" class="state"> Arrived</div>
                    <div id="returningID" class="state"> Returning</div>
                </div>
            </div>
            <div class="target">
                <h2>Target</h2>
                <div class="status-container">
                    <div id="barID" class="bar">HOMEBASE</div>
                </div>
                

            </div>
        </div>


        <div class="tables-container">
            <div class="tables">
                <h2 class="ttl">Table 1</h2>
                <button onclick="Dispatch(1)" class="g-btn">Dispatch</button>
                <div id="table1" class="request">Requesting</div>
            </div>

            <div class="tables">
                <h2 class="ttl">Table 2</h2>
                <button onclick="Dispatch(2)" class="g-btn">Dispatch</button>
                <div id="table2" class="request">Requesting</div>
            </div>

            <div class="tables">
                <h2 class="ttl">Table 3</h2>
                <button onclick="Dispatch(3)" class="g-btn">Dispatch</button>
                <div id="table3" class="request">Requesting</div>
            </div>
        </div>
    </div>
    <div class="cmd-container">
        <button onclick="Dispatch(0)" class="cmd-base return">Return</button>
        <button onclick="cmd('stop')" class="cmd-base stop">Stop</button>
        <button onclick="cmd('resume')" class="cmd-base resume">Resume</button>
    </div>


    <script>

        var returnStatus = false;
        var idleStatus = false;
        var arrivedStatus = false;
        var movingStatus = false;
        const requests = new Set(); // FIXED: Capital S for Set constructor
        let websocket;

        function initWebSocket(){
            console.log('Initializing WebSocket...');
            websocket = new WebSocket('ws://' + window.location.hostname + ':81/');
            
            websocket.onopen = function(event) {
                console.log('WebSocket connected successfully');
            };
            
            websocket.onmessage = function(event) {
                console.log('Received message:', event.data);
                try {
                    const data = JSON.parse(event.data);
                    console.log('Parsed data:', data);
                    
                    if (data.type === 'request') {
                        console.log('Adding requesting class to table' + data.id);
                        document.getElementById('table'+data.id).classList.add('requesting');
                        //requests.add(data.id); // to fix later
                    }
                    // Add more message type handlers here
                    else if (data.type === 'status') {
                        if(data.status==='arrived'){
                            arriveHandler(data.location);
                        }else if(data.status==='returning'){
                            returnStatus = true;
                            idleStatus = false;
                            document.getElementById('barID').textContent = 'HOMEBASE';
                            document.getElementById('returningID').classList.add('returning');
                            document.getElementById('idleID').classList.remove('idle');
                            document.getElementById('movingID').classList.remove('moving');
                            document.getElementById('arrivedID').classList.remove('arrived');
                        }
                    }
                } catch (e) {
                    console.error('Error parsing message:', e);
                    console.log('Raw message was:', event.data);
                }
            };
            
            websocket.onerror = function(error) {
                console.error('WebSocket error:', error);
            };
            
            websocket.onclose = function(event) {
                console.log('WebSocket closed. Code:', event.code, 'Reason:', event.reason);
                // Optionally reconnect after a delay
                setTimeout(initWebSocket, 3000);
            };
        }

        function Dispatch(t_ID){
            console.log('Dispatching to table:', t_ID);
            movingStatus = true;
            idleStatus = false;
            
            // Send dispatch command to server
            if (websocket && websocket.readyState === WebSocket.OPEN) {
                const message = JSON.stringify({
                    type: 'dispatch',
                    tableId: t_ID
                });
                websocket.send(message);
                console.log('Sent dispatch command:', message);
            }
            
            switch(t_ID) {
                case 1:
                    document.getElementById('barID').textContent = 'TABLE 1';
                    document.getElementById('movingID').classList.add('moving');
                    document.getElementById('table1').classList.remove('requesting');
                    
                    returnStatus = false;
                    //requests.delete(1);
                    break;
                case 2:
                    document.getElementById('barID').textContent = 'TABLE 2';
                    document.getElementById('movingID').classList.add('moving');
                    document.getElementById('table2').classList.remove('requesting');
                    returnStatus = false;
                    //requests.delete(2);
                    break;
                case 3:
                    document.getElementById('barID').textContent = 'TABLE 3';
                    document.getElementById('movingID').classList.add('moving');
                    document.getElementById('table3').classList.remove('requesting');
                    returnStatus = false;
                    //requests.delete(3);
                    break;
                case 0:
                    returnStatus = true;
                    idleStatus = false;
                    document.getElementById('barID').textContent = 'HOMEBASE';
                    document.getElementById('returningID').classList.add('returning');
                    document.getElementById('idleID').classList.remove('idle');
                    document.getElementById('movingID').classList.remove('moving');
                    document.getElementById('arrivedID').classList.remove('arrived');
                    break;
                default:
                    document.getElementById('barID').textContent = 'HOMEBASE';
            }
            
            // Clear other status classes
            document.getElementById('idleID').classList.remove('idle');
            document.getElementById('arrivedID').classList.remove('arrived');
            document.getElementById('returningID').classList.remove('returning');
        }

        function cmd(cmdName) {
            console.log('Executing command:', cmdName);
            
            // Send command to server
            if (websocket && websocket.readyState === WebSocket.OPEN) {
                const message = JSON.stringify({
                    type: 'command',
                    command: cmdName
                });
                websocket.send(message);
                console.log('Sent command:', message);
            }
            
            switch(cmdName) {
                
                case 'stop':
                    movingStatus = false;
                    idleStatus = true;
                    document.getElementById('idleID').classList.add('idle');
                    document.getElementById('movingID').classList.remove('moving');
                    document.getElementById('arrivedID').classList.remove('arrived');
                    document.getElementById('returningID').classList.remove('returning');
                    break;
                case 'resume':
                    if(returnStatus){
                        movingStatus = false;
                        idleStatus = false;
                        document.getElementById('returningID').classList.add('returning');
                        document.getElementById('barID').textContent = 'HOMEBASE';
                        document.getElementById('movingID').classList.remove('moving');
                    }
                    else{
                        movingStatus = true;
                        idleStatus = false;
                        document.getElementById('movingID').classList.add('moving');
                        document.getElementById('returningID').classList.remove('returning');
                    }
                    document.getElementById('idleID').classList.remove('idle');
                    document.getElementById('arrivedID').classList.remove('arrived');
                    break;
                default:
                    break;
            }
        }

        function arriveHandler(location){
            if(location === 'homebase'){
                console.log('Robot returned');
                returnStatus = false;
                idleStatus = true;
                document.getElementById('idleID').classList.remove('idle');
                document.getElementById('movingID').classList.remove('moving');
                document.getElementById('arrivedID').classList.add('arrived');
                document.getElementById('returningID').classList.remove('returning');
            }
            else{
                console.log('Robot arrived');
                returnStatus = false;
                idleStatus = true;
                document.getElementById('idleID').classList.add('idle');
                document.getElementById('movingID').classList.remove('moving');
                document.getElementById('arrivedID').classList.remove('arrived');
                document.getElementById('returningID').classList.remove('returning');
                
            }
        }

        // Initialize WebSocket connection when page loads
        window.addEventListener('load', function() {
            console.log('Page loaded, initializing WebSocket...');
            initWebSocket();
        });
    </script>
</body>
</html>
)rawliteral";