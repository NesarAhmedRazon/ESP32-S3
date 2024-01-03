// Create a new index file in the src folder and name it index.h. Copy the following code into the file:
// on load send a request to the server to get all the effects from server response json data and make a button for each effect and font the button add a id with the effect name and data-var with the effect size
// on click of a button add a class 'current' to clicked button, send a request to the server to send the effect name and update the input range max to the data-var value and set the input range value to 0, and set the slider-value text to 0
// on input of the input range send a request to the server to send the speed value and the current effect name and update the slider-value text to the input range value
// Path: src/index.h

const char home_page[] = R"rawliteral(<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width,initial-scale=1.0'>
    <title>Speed Slider</title>
</head>
<body>
    <div class='container'>
        <div class='slider-container'>
            <input type='range' min='0' max='100' value='0' class='slider' id='myRange'>
            <p>Value: <span id='slider-value'></span></p>
        </div>
        <div class='buttons-container'>
        </div>
    </div>
    <style>
        body {
            margin: 0;
            padding: 0;
            font-family: sans-serif;
        }

        .container {
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .slider-container {
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            width: 100%;
        }

        .slider {
            -webkit-appearance: none;
            width: 100%;
            height: 25px;
            border-radius: 5px;
            background: #d3d3d3;
            outline: none;
            opacity: 0.7;
            -webkit-transition: .2s;
            transition: opacity .2s;
        }

        .slider:hover {
            opacity: 1;
        }

        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            border-radius: 50%;
            background: #4CAF50;
            cursor: pointer;
        }

        .slider::-moz-range-thumb {
            width: 25px;
            height: 25px;
            border-radius: 50%;
            background: #4CAF50;
            cursor: pointer;
        }

        .buttons-container {
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            align-items: center;
            width: 100%;
        }

        .button {
            display: flex;
            justify-content: center;
            align-items: center;
            width: 100px;
            height: 50px;
            margin: 10px;
            border-radius: 5px;
            background: #d3d3d3;
            cursor: pointer;
        }

        .button:hover {
            background: #4CAF50;
        }

        .button.current {
            background: #4CAF50;
        }

        .button.current:hover {
            background: #d3d3d3;
        }
    </style>

    <script>
        const buttonsContainer = document.querySelector('.buttons-container');
        const slider = document.getElementById('myRange');
        const sliderValue = document.getElementById('slider-value');

        // on load send a request to the server to get all the effects from server response json data and make a button for each effect and font the button add a id with the effect name and data-var with the effect size
        window.addEventListener('load', () => {
            fetch('/effects')
                .then(response => response.json())
                .then(data => {
                    for (let effect in data) {
                        const button = document.createElement('button');
                        button.classList.add('button');
                        button.innerText = effect;
                        button.id = effect;
                        button.dataset.var = data[effect].length;
                        button.addEventListener('click', () => {
                            // on click of a button add a class 'current' to clicked button, send a request to the server to send the effect name and update the input range max to the data-var value and set the input range value to 0, and set the slider-value text to 0
                            const current = document.querySelector('.current');
                            if (current) {
                                current.classList.remove('current');
                            }
                            button.classList.add('current');
                            fetch(`/get-effect?effect=${effect}`)
                                .then(response => response.json())
                                .then(data => {
                                    slider.max = data.var;
                                    slider.value = 0;
                                    sliderValue.innerText = 0;
                                });
                        });
                        buttonsContainer.appendChild(button);
                    }
                }).then(data=>{
                    // log the data
                    console.log(data);
                })
                .catch(error => console.log(error)).finally(() => {
                    // on load click the first button
                    const firstButton = document.querySelector('.button');
                    firstButton.click();
                });
        });

        // on input of the input range send a request to the server to send the speed value and the current effect name and update the slider-value text to the input range value
        slider.addEventListener('input', () => {
            fetch(`/get-effect?var=${slider.value}&effect=${document.querySelector('.current').id}`)
                .then(response => response.json())
                .then(data => {
                    sliderValue.innerText = slider.value;
                });
        });
    
    
</body>
</html>)rawliteral";
