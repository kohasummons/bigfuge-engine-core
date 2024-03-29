#include <ESPmDNS.h>

#include <WiFi.h>                                     
#include <WebServer.h>                                
#include <WebSocketsServer.h>                         

#include <ArduinoJson.h>                              
#include <ESP32Servo.h>                             

#define AP_SSID "BigFuge Xela-8"
#define AP_PASS "0xBigBiomedical"

IPAddress Actual_IP;

IPAddress PageIP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 184);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;

// The String below "webpage" contains the complete HTML code that is sent to the client whenever someone connects to the webserver
String webpage = "<!DOCTYPE html><html lang='en'> <head> <meta charset='UTF-8'/> <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <title>Bigfuge</title> <style>*, *::after, *::before{box-sizing: border-box; margin: 0; padding: 0;}:root{--grey-100: #e4e1dd; --grey-200: #b3b3b3; --max-width: 600px; --text-gray: var(--grey-200); --hot-red: #ff520e;}html, body{background-color: var(--grey-100); margin: auto 0; font-family: sans-serif; user-select: none;}img{width: 100%; height: 100%; object-fit: contain;}.container{max-width: var(--max-width); height: 100vh; margin-right: auto; margin-left: auto; position: relative;}.panel{height: 100vh; max-height: 100vh; width: var(--max-width); max-width: 100%; position: fixed; animation-duration: 1s; animation-fill-mode: forwards; animation-timing-function: linear;}.detach{opacity: 0; visibility: hidden; z-index: -100;}#preloader{display: flex; flex-direction: column;}#preloader > .header{display: flex; align-items: center; justify-content: center; height: 90vh;}#preloader .header > img{width: 200px;}#preloader .header > .bigfuge-identity{width: 200px; display: flex; align-items: center; color: var(--hot-red);}#preloader > .footer{text-align: center; color: var(--text-gray);}#home{padding: 8px 8px; display: flex; flex-direction: column;}#home .header{display: flex; flex-direction: row; justify-content: space-between; align-items: center;}#home .header img{width: 200px;}#home .header .bigfuge-identity{width: 200px; height: 10px; display: flex; align-items: center; color: var(--hot-red);}#home .header .header_network{color: var(--grey-200); width: 40px; height: 40px; display: flex; align-items: center; justify-content: center; background: #dddddd; border-radius: 100%;}#home .header .header_network.connected{color: #7ddb26;}#home .header .header_network.disconnected{color: #db4444;}#home .notif{display: grid; grid-template-columns: auto auto; gap: 12px; margin-top: 50px;}#home .notif > .notif-item{background-color: #eeeeee; height: fit-content; align-self: flex-end; padding: 1rem 1rem; border-radius: 12px;}#home .notif > .notif-item > span{font-weight: bold; font-size: 0.85em; color: var(--grey-200);}#home .notif > .notif-item > span#speedIn{display: inline-block; font-size: 0.75em; font-weight: normal;}#home .notif > .notif-item > p{font-size: 2rem; margin-top: 0.5rem; color: #4d4d4d;}#home .controls{height: 50%; display: flex; flex-direction: column; justify-content: end;}#home .controls > button{width: 100%; padding: 1.5rem; margin-top: 1.25rem; outline: none; border: none; color: #eee; font-size: 1.25rem; border-radius: 0.25rem; cursor: pointer;}#home .controls > button.start{background-color: rgb(221, 97, 14);}#home .controls > button.stop{background-color: rgb(221, 14, 14);}#home .controls > button:hover{opacity: 95%; transition-property: all; transition-duration: 2000;}.config-speed{display: flex; justify-content: center; align-items: center; flex-direction: column; font-size: 3em; min-height: 100px; gap: 0.2em;}.config-speed__header{font-size: 0.45em; font-weight: normal; align-self: flex-start; translate: 25px; color: var(--grey-200);}.config-time{display: flex; justify-content: center; align-items: center; flex-direction: column; font-size: 3em; min-height: 100px; gap: 0.2em; margin-top: 0.75em;}.config-time__header{font-size: 0.45em; font-weight: normal; align-self: flex-start; translate: 25px; color: var(--grey-200);}.config-time__wrapper{width: 100%;}.config-time__chip-wrapper{list-style-type: none; display: flex; flex-direction: row; overflow: auto; white-space: nowrap; gap: 10px;}.config-time__chip-wrapper > li{font-size: 0.75em; word-spacing: -5.95px; background-color: #f7f7f7; padding: 0.25em 0.45em; border-radius: 12px; cursor: pointer;}.config-time__chip-wrapper > li.disabled{opacity: 30%;}.config-time__chip-wrapper > li:nth-child(1){margin-left: 0.55em;}.config-time__chip-wrapper > li.active{border: 1px solid #1e3ffb;}.config-time__chip-wrapper > li > span{font-size: 0.55em;}.b{border: 1px solid red; background-color: #fae7cfe1;}.hide{display: none;}.hide-scrollbar::-webkit-scrollbar{display: none;}.blink{animation: blink 800ms infinite;}@keyframes blink{0%{opacity: 1;}50%{opacity: 0;}100%{opacity: 1;}}.slider-wrapper{--slider-height: 0.3125em; --slider-thumb-size: 1.75em; --slider-progress: 50; border-radius: 50px; width: clamp(9em, 8em, 100%); max-width: 100%; transition: width 0.2s;}.slider-wrapper.unlocked{width: 8em;}.slider-track{display: flex; justify-content: center; align-items: center; position: relative; border-radius: inherit; width: 100%; height: var(--slider-height); background-image: linear-gradient( to right, transparent 0% calc(var(--slider-progress) * 1%), #aaa calc(var(--slider-progress) * 1%) 100% ), linear-gradient(to right, #62ad1d, #fbbd1e, #fb6e1e, #e71a1a, #220000); background-size: auto, 125% 100%; box-shadow: inset 0 2px 4px rgba(255, 255, 255, 0.6), inset 0 -2px 4px rgba(0, 0, 0, 0.1), 0 2px 4px rgba(0, 0, 0, 0.1); transition: background-size 0.2s;}.slider-wrapper.unlocked > .slider-track{background-size: auto, 100% 100%;}.slider-input{-moz-appearance: none; -webkit-appearance: none; appearance: none; position: absolute; z-index: 1; left: 50%; transform: translate(-50%); border-radius: inherit; width: calc(100% + var(--slider-thumb-size)); height: 100%; opacity: 0; cursor: pointer;}.slider-input::-webkit-slider-runnable-track{height: 100%;}.slider-input::-webkit-slider-thumb{-moz-appearance: none; -webkit-appearance: none; appearance: none; margin-top: calc( (var(--slider-height) / 2) - (var(--slider-thumb-size) / 2) ); border-width: 0; border-radius: 50%; width: var(--slider-thumb-size); height: var(--slider-thumb-size);}.slider-input::-moz-range-thumb{-moz-appearance: none; -webkit-appearance: none; appearance: none; margin-top: calc( (var(--slider-height) / 2) - (var(--slider-thumb-size) / 2) ); border-width: 0; border-radius: 50%; width: var(--slider-thumb-size); height: var(--slider-thumb-size);}.slider-thumb{display: flex; justify-content: center; align-items: center; position: absolute; left: calc( var(--slider-progress) / 100 * (100% - var(--slider-height)) + var(--slider-height) / 2 ); left: calc(var(--slider-progress) * 1%); transform: translateX(-50%); width: var(--slider-thumb-size); height: var(--slider-thumb-size); transition: transform 0.2s;}.slider-input:active + .slider-thumb{transform: translateX(-50%) scale(0.86);}.slider-thumb::after{content: ''; position: absolute; z-index: 1; width: 150%; height: 150%; background-image: url('https://assets.codepen.io/4175254/explosion-2.png'); background-size: 800%; opacity: 0;}.slider-thumb.play-explosion::after{animation: explosion 0.5s steps(8) forwards; opacity: 1;}@keyframes explosion{to{background-position-x: -800%;}}.slider-thumb-image{--slider-thumb-image-color: rgb(251 150 30); position: relative; width: 100%; height: 100%; fill: var(--slider-thumb-image-color); transition: transform 0.2s;}.slider-thumb-image.shake{animation: shake 0.2s infinite;}@keyframes shake{0%{transform: rotate(0deg);}25%{transform: rotate(5deg);}50%{transform: rotate(0eg);}75%{transform: rotate(-5deg);}100%{transform: rotate(0deg);}}@keyframes fadeOut{from{opacity: 1;}to{opacity: 0;}}@keyframes fadeIn{from{opacity: 0;}to{opacity: 1;}}.hide{display: none !important;}</style> </head> <body> <main class='container'> <section id='preloader' class='panel'> <div class='header'> <div class='bigfuge-identity'> <svg fill='currentColor' xmlns='http://www.w3.org/2000/svg' width='1286.6667' height='344' version='1.0' viewBox='0 0 965 258' > <path d='M350 64.2c-.6 2.4-2 7.3-3 11.1l-1.9 6.7 18.1-.2 18.1-.3 2.8-10c1.5-5.5 2.8-10.3 2.9-10.8 0-.4-8.1-.7-18-.7h-17.9l-1.1 4.2zM110.4 65.3c-7.2 6.6-5 17.4 4.3 20.6 12.6 4.4 21.8-11.5 11.9-20.6-2.9-2.7-4.4-3.3-8.1-3.3-3.7 0-5.2.6-8.1 3.3zM237.4 67.7c-.3 1.5-1.4 5.7-2.4 9.3-1.8 6.2-17.4 64.2-19 70.5-.4 1.6-2.2 8.1-3.9 14.3-1.7 6.2-3.1 11.5-3.1 11.7 0 .3 15.2.5 33.8.5 37.7 0 47.9-1 58.5-5.8 8.2-3.8 13-8.1 16.3-14.8 2.3-4.6 2.6-6.4 2.2-12.7-.4-9.6-3.2-14-11.4-18.4l-5.9-3.2 3-1.1c15-5.6 22.5-14.9 22.5-27.8 0-11.1-5.8-18.4-18-22.4-6-2-9-2.2-39.2-2.6l-32.7-.4-.7 2.9zm53.3 25.8c2.8 1.9 3.3 4.6 1.7 8.4-2 4.7-6.3 6.1-19.4 6.1-9.8 0-11-.2-11-1.8 0-1.5 2.1-9.1 3.5-13 .7-1.9 22.4-1.7 25.2.3zm-8.8 38.9c4.2 3.8 1.7 11.3-4.6 13.7-1.3.5-7.9.9-14.5.9-12 0-12 0-11.4-2.3.3-1.2 1.4-5 2.3-8.5l1.6-6.4 12.2.4c10.4.2 12.6.6 14.4 2.2zM506 65.4c0 .3-2.3 8.9-5.1 19.3-2.8 10.3-5.5 20.1-5.9 21.8-1 3.9-17 63.5-17.6 65.7-.5 1.7.8 1.8 17.3 1.8h17.9l3.9-14.8c2.1-8.1 4.2-16.2 4.7-18l.9-3.2h51.7l3-11.3c1.7-6.1 3.4-12.3 3.7-13.7l.6-2.5-25.6.3c-20.2.2-25.5 0-25.5-1 .1-.7.9-4.2 1.9-7.8l1.8-6.5 28.2-.3c21.5-.2 28.1-.5 28.1-1.5 0-1.3 4.3-17.4 6.5-24.5l1.4-4.2h-46c-25.2 0-45.9.2-45.9.4zM411.7 88.5c-19.3 4.9-32.7 24.3-32.7 47.4 0 10.6 2.5 16.8 9.3 22.8 5.5 4.7 11.7 6.8 20.2 6.7 7.9-.1 13.3-1.7 19.4-5.9 4.1-2.7 5.1-3 5.1-1.7-.1 5.5-7.7 15-13.9 17.3-1.4.6-6.5 1-11.3 1-7.1 0-10.3-.6-17.5-3.1-4.8-1.7-9.5-3.3-10.3-3.5-1-.4-3.5 2.4-7.8 8.7-3.4 5.1-6.2 9.6-6.2 9.9 0 1.2 11.8 5.9 19.5 7.9 16.5 4.2 29 4.6 44.8 1.4 19.8-4 30.2-16.1 37.7-43.9 1.2-4.4 3-10.9 4-14.5 2.5-9 11.8-43.9 12.6-47.3l.6-2.7-17.3.2-17.4.3-1.2 4.9c-.7 2.6-1.4 5.1-1.7 5.3-.3.3-1.7-.8-3.1-2.5-1.5-1.7-5.4-4.5-8.8-6.2-7.6-3.7-15.8-4.6-24-2.5zm23.9 27.1c5.7 3.8 6.6 12.8 2 19.6-8.3 12.4-25.6 9.3-25.6-4.7 0-7 5.3-14.5 11.6-16.5 3.5-1.1 8.9-.4 12 1.6zM723.2 88c-11.5 2.4-22.7 11-28.5 21.7-1.5 2.8-3.6 8.6-4.7 12.9-5 19.3-.2 33.2 13.8 40.1 4.7 2.3 7 2.8 13.2 2.7 8.6 0 13.7-1.4 19.8-5.5 2.4-1.6 4.7-2.9 5.2-2.9 1.2 0-1.5 7.5-4 11.1-1.2 1.6-4.1 4.1-6.5 5.5-3.9 2.3-5.5 2.6-13.9 2.5-8 0-10.9-.5-18.2-3-4.8-1.7-9.2-3.5-9.7-4-.6-.6-3.5 2.6-7.8 9-3.8 5.5-6.9 10.1-6.9 10.3 0 .2 3.4 1.8 7.6 3.6 12 5 24 7.3 37.9 7.3 19.8 0 32.4-4.2 41.4-13.8 7.6-8 11.9-18 18-41 1.2-4.4 4.6-17.2 7.7-28.5 3-11.3 5.9-22 6.4-23.8l.8-3.2h-34.5l-1.3 4.7c-.7 2.7-1.5 5.1-1.7 5.6-.3.4-2.7-1.2-5.4-3.7-7.4-6.7-18.7-9.7-28.7-7.6zm21.5 27.6c5.8 3.7 6.7 13.2 1.9 20.5-4.7 7-14.8 9.1-20.5 4.3-4.5-3.8-5.8-8.9-3.7-15.1 1.9-5.8 4.9-8.9 10.3-10.9 4.1-1.4 8.5-1 12 1.2zM831.2 88.5c-9.6 2.2-16.2 5.6-23.4 12.4-22.5 21-22 56.9 1 69.4 10.1 5.5 25.6 7.3 37 4.3 6.5-1.8 16.8-6.7 20.4-9.8l2.8-2.3-7.8-8.3-7.7-8.4-6.5 3.1c-8.1 3.9-14.2 4.1-19.1.8-3.2-2.1-6.3-8.1-5.2-10 .2-.4 12.7-.7 27.8-.7 31.4 0 28.4 1.2 29.2-11.6 1.2-18.5-6.3-32.1-20.6-37.5-7.2-2.7-19.6-3.3-27.9-1.4zm15.5 24c2.6 1.8 4.7 7.7 3.8 10.5-.6 1.9-1.5 2-13.1 2-13.8 0-14.1-.2-10.1-6.1 4.8-7.1 14-10.2 19.4-6.4zM343 92.7c-.6 2.1-5.7 21.1-11.4 42.3l-10.2 38.5 16.9.3c9.4.1 17.2 0 17.5-.2.2-.3 1-2.9 1.7-5.8s5.6-21.4 10.9-41c5.3-19.6 9.6-36.2 9.6-36.8 0-.6-5.9-1-17-1h-16.9l-1.1 3.7zM599.5 90.2c-.2.7-3.4 12.5-7.1 26.3-7.6 28.2-8.5 36.8-5.1 45.7 6.1 15.9 28.6 18.3 44 4.8 4.1-3.6 4.9-3.7 3.8-.8-.4 1.3-1.1 3.5-1.5 5.1l-.6 2.7h34.6l8.4-31.3c4.6-17.1 9.4-35.3 10.8-40.2 1.3-5 2.7-10 2.9-11.3l.5-2.2-17.3.2-17.3.3-6.3 23.4c-6.6 24.5-8.6 29.2-13.3 31.6-6.5 3.4-14 0-14-6.2 0-1.5 2.7-12.9 6-25.3s6-22.8 6-23.3c0-.4-7.7-.7-17-.7-12.8 0-17.2.3-17.5 1.2zM156.2 103.2c-5 1.7-10.9 8.7-11.7 14-1.9 12.8 6.9 22.5 19.7 21.6 22.2-1.6 22.4-34.4.2-36.3-2.7-.2-6.4.1-8.2.7zM75.7 109.5c-4.8 1.8-11.9 6.9-14.4 10.4-5.5 7.4-6.8 11.3-6.8 20.6 0 8.4.2 9.4 3.4 15.3 3.5 6.4 8.5 11.3 15.1 14.8 2.8 1.5 5.6 1.9 14 1.9 9.8 0 10.9-.2 15.9-3 6.1-3.3 11-8.6 14.3-15.4 3.4-7 3.3-19.6-.3-27.2-3.3-6.9-8.9-12.7-15.5-16-6-3-19.6-3.7-25.7-1.4zM898.3 150.1c-5.6 1.6-9.2 8.7-7.3 14.3 1.5 4.6 5.1 7.7 9.7 8.3 13.8 1.8 18.6-16.9 5.7-22.3-3.7-1.6-3.7-1.6-8.1-.3zm7.4.9c.8 0 2.8 1.4 4.4 3.1 4.2 4.6 4.1 9.9-.5 14.5-2.5 2.5-4.3 3.4-6.7 3.4-5.1 0-9.6-3.2-11-7.9-1-3.4-.9-4.4.7-7.3 2.1-3.5 8-7.2 10.2-6.3.8.3 2 .5 2.9.5z'/> <path d='M898.1 161.2c.1 3.5.4 5.1.6 3.5.4-2.2.9-2.7 3.3-2.7 2.2 0 3 .6 3.8 2.7.8 2.4.9 1.8 1-3.5V155H898l.1 6.2zm7.6-3.6c.7 1.9-1.4 3.5-4.2 3.2-1.2-.2-2.1-1.2-2.3-2.6-.3-1.9.1-2.2 2.8-2.2 1.9 0 3.3.6 3.7 1.6z'/> </svg> </div></div><div class='footer'> <span>BigBiomedicals</span> </div></section> <section id='home' class='panel detach'> <div class='header'> <div class='header_icon'> <div class='bigfuge-identity'> <svg fill='currentColor' xmlns='http://www.w3.org/2000/svg' width='1286.6667' height='344' version='1.0' viewBox='0 0 965 258' > <path d='M350 64.2c-.6 2.4-2 7.3-3 11.1l-1.9 6.7 18.1-.2 18.1-.3 2.8-10c1.5-5.5 2.8-10.3 2.9-10.8 0-.4-8.1-.7-18-.7h-17.9l-1.1 4.2zM110.4 65.3c-7.2 6.6-5 17.4 4.3 20.6 12.6 4.4 21.8-11.5 11.9-20.6-2.9-2.7-4.4-3.3-8.1-3.3-3.7 0-5.2.6-8.1 3.3zM237.4 67.7c-.3 1.5-1.4 5.7-2.4 9.3-1.8 6.2-17.4 64.2-19 70.5-.4 1.6-2.2 8.1-3.9 14.3-1.7 6.2-3.1 11.5-3.1 11.7 0 .3 15.2.5 33.8.5 37.7 0 47.9-1 58.5-5.8 8.2-3.8 13-8.1 16.3-14.8 2.3-4.6 2.6-6.4 2.2-12.7-.4-9.6-3.2-14-11.4-18.4l-5.9-3.2 3-1.1c15-5.6 22.5-14.9 22.5-27.8 0-11.1-5.8-18.4-18-22.4-6-2-9-2.2-39.2-2.6l-32.7-.4-.7 2.9zm53.3 25.8c2.8 1.9 3.3 4.6 1.7 8.4-2 4.7-6.3 6.1-19.4 6.1-9.8 0-11-.2-11-1.8 0-1.5 2.1-9.1 3.5-13 .7-1.9 22.4-1.7 25.2.3zm-8.8 38.9c4.2 3.8 1.7 11.3-4.6 13.7-1.3.5-7.9.9-14.5.9-12 0-12 0-11.4-2.3.3-1.2 1.4-5 2.3-8.5l1.6-6.4 12.2.4c10.4.2 12.6.6 14.4 2.2zM506 65.4c0 .3-2.3 8.9-5.1 19.3-2.8 10.3-5.5 20.1-5.9 21.8-1 3.9-17 63.5-17.6 65.7-.5 1.7.8 1.8 17.3 1.8h17.9l3.9-14.8c2.1-8.1 4.2-16.2 4.7-18l.9-3.2h51.7l3-11.3c1.7-6.1 3.4-12.3 3.7-13.7l.6-2.5-25.6.3c-20.2.2-25.5 0-25.5-1 .1-.7.9-4.2 1.9-7.8l1.8-6.5 28.2-.3c21.5-.2 28.1-.5 28.1-1.5 0-1.3 4.3-17.4 6.5-24.5l1.4-4.2h-46c-25.2 0-45.9.2-45.9.4zM411.7 88.5c-19.3 4.9-32.7 24.3-32.7 47.4 0 10.6 2.5 16.8 9.3 22.8 5.5 4.7 11.7 6.8 20.2 6.7 7.9-.1 13.3-1.7 19.4-5.9 4.1-2.7 5.1-3 5.1-1.7-.1 5.5-7.7 15-13.9 17.3-1.4.6-6.5 1-11.3 1-7.1 0-10.3-.6-17.5-3.1-4.8-1.7-9.5-3.3-10.3-3.5-1-.4-3.5 2.4-7.8 8.7-3.4 5.1-6.2 9.6-6.2 9.9 0 1.2 11.8 5.9 19.5 7.9 16.5 4.2 29 4.6 44.8 1.4 19.8-4 30.2-16.1 37.7-43.9 1.2-4.4 3-10.9 4-14.5 2.5-9 11.8-43.9 12.6-47.3l.6-2.7-17.3.2-17.4.3-1.2 4.9c-.7 2.6-1.4 5.1-1.7 5.3-.3.3-1.7-.8-3.1-2.5-1.5-1.7-5.4-4.5-8.8-6.2-7.6-3.7-15.8-4.6-24-2.5zm23.9 27.1c5.7 3.8 6.6 12.8 2 19.6-8.3 12.4-25.6 9.3-25.6-4.7 0-7 5.3-14.5 11.6-16.5 3.5-1.1 8.9-.4 12 1.6zM723.2 88c-11.5 2.4-22.7 11-28.5 21.7-1.5 2.8-3.6 8.6-4.7 12.9-5 19.3-.2 33.2 13.8 40.1 4.7 2.3 7 2.8 13.2 2.7 8.6 0 13.7-1.4 19.8-5.5 2.4-1.6 4.7-2.9 5.2-2.9 1.2 0-1.5 7.5-4 11.1-1.2 1.6-4.1 4.1-6.5 5.5-3.9 2.3-5.5 2.6-13.9 2.5-8 0-10.9-.5-18.2-3-4.8-1.7-9.2-3.5-9.7-4-.6-.6-3.5 2.6-7.8 9-3.8 5.5-6.9 10.1-6.9 10.3 0 .2 3.4 1.8 7.6 3.6 12 5 24 7.3 37.9 7.3 19.8 0 32.4-4.2 41.4-13.8 7.6-8 11.9-18 18-41 1.2-4.4 4.6-17.2 7.7-28.5 3-11.3 5.9-22 6.4-23.8l.8-3.2h-34.5l-1.3 4.7c-.7 2.7-1.5 5.1-1.7 5.6-.3.4-2.7-1.2-5.4-3.7-7.4-6.7-18.7-9.7-28.7-7.6zm21.5 27.6c5.8 3.7 6.7 13.2 1.9 20.5-4.7 7-14.8 9.1-20.5 4.3-4.5-3.8-5.8-8.9-3.7-15.1 1.9-5.8 4.9-8.9 10.3-10.9 4.1-1.4 8.5-1 12 1.2zM831.2 88.5c-9.6 2.2-16.2 5.6-23.4 12.4-22.5 21-22 56.9 1 69.4 10.1 5.5 25.6 7.3 37 4.3 6.5-1.8 16.8-6.7 20.4-9.8l2.8-2.3-7.8-8.3-7.7-8.4-6.5 3.1c-8.1 3.9-14.2 4.1-19.1.8-3.2-2.1-6.3-8.1-5.2-10 .2-.4 12.7-.7 27.8-.7 31.4 0 28.4 1.2 29.2-11.6 1.2-18.5-6.3-32.1-20.6-37.5-7.2-2.7-19.6-3.3-27.9-1.4zm15.5 24c2.6 1.8 4.7 7.7 3.8 10.5-.6 1.9-1.5 2-13.1 2-13.8 0-14.1-.2-10.1-6.1 4.8-7.1 14-10.2 19.4-6.4zM343 92.7c-.6 2.1-5.7 21.1-11.4 42.3l-10.2 38.5 16.9.3c9.4.1 17.2 0 17.5-.2.2-.3 1-2.9 1.7-5.8s5.6-21.4 10.9-41c5.3-19.6 9.6-36.2 9.6-36.8 0-.6-5.9-1-17-1h-16.9l-1.1 3.7zM599.5 90.2c-.2.7-3.4 12.5-7.1 26.3-7.6 28.2-8.5 36.8-5.1 45.7 6.1 15.9 28.6 18.3 44 4.8 4.1-3.6 4.9-3.7 3.8-.8-.4 1.3-1.1 3.5-1.5 5.1l-.6 2.7h34.6l8.4-31.3c4.6-17.1 9.4-35.3 10.8-40.2 1.3-5 2.7-10 2.9-11.3l.5-2.2-17.3.2-17.3.3-6.3 23.4c-6.6 24.5-8.6 29.2-13.3 31.6-6.5 3.4-14 0-14-6.2 0-1.5 2.7-12.9 6-25.3s6-22.8 6-23.3c0-.4-7.7-.7-17-.7-12.8 0-17.2.3-17.5 1.2zM156.2 103.2c-5 1.7-10.9 8.7-11.7 14-1.9 12.8 6.9 22.5 19.7 21.6 22.2-1.6 22.4-34.4.2-36.3-2.7-.2-6.4.1-8.2.7zM75.7 109.5c-4.8 1.8-11.9 6.9-14.4 10.4-5.5 7.4-6.8 11.3-6.8 20.6 0 8.4.2 9.4 3.4 15.3 3.5 6.4 8.5 11.3 15.1 14.8 2.8 1.5 5.6 1.9 14 1.9 9.8 0 10.9-.2 15.9-3 6.1-3.3 11-8.6 14.3-15.4 3.4-7 3.3-19.6-.3-27.2-3.3-6.9-8.9-12.7-15.5-16-6-3-19.6-3.7-25.7-1.4zM898.3 150.1c-5.6 1.6-9.2 8.7-7.3 14.3 1.5 4.6 5.1 7.7 9.7 8.3 13.8 1.8 18.6-16.9 5.7-22.3-3.7-1.6-3.7-1.6-8.1-.3zm7.4.9c.8 0 2.8 1.4 4.4 3.1 4.2 4.6 4.1 9.9-.5 14.5-2.5 2.5-4.3 3.4-6.7 3.4-5.1 0-9.6-3.2-11-7.9-1-3.4-.9-4.4.7-7.3 2.1-3.5 8-7.2 10.2-6.3.8.3 2 .5 2.9.5z'/> <path d='M898.1 161.2c.1 3.5.4 5.1.6 3.5.4-2.2.9-2.7 3.3-2.7 2.2 0 3 .6 3.8 2.7.8 2.4.9 1.8 1-3.5V155H898l.1 6.2zm7.6-3.6c.7 1.9-1.4 3.5-4.2 3.2-1.2-.2-2.1-1.2-2.3-2.6-.3-1.9.1-2.2 2.8-2.2 1.9 0 3.3.6 3.7 1.6z'/> </svg> </div></div><div class='header_network'> <svg xmlns='http://www.w3.org/2000/svg' width='20' height='20' viewBox='0 0 24 24' > <path fill='currentColor' d='M12,21L15.6,16.2C14.6,15.45 13.35,15 12,15C10.65,15 9.4,15.45 8.4,16.2L12,21' opacity='0' > <animate id='svgSpinnersWifi0' fill='freeze' attributeName='opacity' begin='0;svgSpinnersWifi1.end+0.2s' calcMode='discrete' dur='0.25s' values='0;1'/> <animate id='svgSpinnersWifi1' fill='freeze' attributeName='opacity' begin='svgSpinnersWifi3.end+0.5s' dur='0.001s' values='1;0'/> </path> <path fill='currentColor' d='M12,9C9.3,9 6.81,9.89 4.8,11.4L6.6,13.8C8.1,12.67 9.97,12 12,12C14.03,12 15.9,12.67 17.4,13.8L19.2,11.4C17.19,9.89 14.7,9 12,9Z' opacity='0' > <animate id='svgSpinnersWifi2' fill='freeze' attributeName='opacity' begin='svgSpinnersWifi0.end' calcMode='discrete' dur='0.25s' values='0;1'/> <animate fill='freeze' attributeName='opacity' begin='svgSpinnersWifi3.end+0.5s' dur='0.001s' values='1;0'/> </path> <path fill='currentColor' d='M12,3C7.95,3 4.21,4.34 1.2,6.6L3,9C5.5,7.12 8.62,6 12,6C15.38,6 18.5,7.12 21,9L22.8,6.6C19.79,4.34 16.05,3 12,3' opacity='0' > <animate id='svgSpinnersWifi3' fill='freeze' attributeName='opacity' begin='svgSpinnersWifi2.end' calcMode='discrete' dur='0.25s' values='0;1'/> <animate fill='freeze' attributeName='opacity' begin='svgSpinnersWifi3.end+0.5s' dur='0.001s' values='1;0'/> </path> </svg> </div></div><div class='notif'> <div class='notif-item notif-power'> <span>Battery Power (v)</span> <p>11</p></div><div class='notif-item notif-status'> <span>Status</span> <p>Idle</p></div><div class='notif-item notif-speed'> <span>Speed (RPM)</span> <p>500</p><span id='speedIn' class='hide'>500</span> </div><div class='notif-item notif-timer'> <span>Timer (mm:ss)</span> <p>00:10</p></div></div><div class='config'> <div class='config-speed'> <div class='config-speed__header'> <h4>Set Speed</h4> </div><div class='slider-wrapper'> <div class='slider-track'> <input class='slider-input' type='range' value='100'/> <div class='slider-thumb'> <svg class='slider-thumb-image' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 176 176' > <path d='M109.621 58.624c-2.248-1.8-5.008-2.962-7.793-3.69-55.136-14.399-18.22 90.985-88.17 59.012-.306-.14-.621-.334-.907-.511-3.895-2.411-8.839 1.419-6.676 5.669 37.499 73.676 140.46-6.692 111.993-50.336-1.504-2.305-3.109-4.594-4.889-6.692a25.95 25.95 0 0 0-3.558-3.453z' filter='url(#A)'/> <path d='M108.983 52.653c-15.728-4.464-34.22 2.06-36.123 4.369 14.141-.161 22.213-.751 29.185 10.25 12.719-.852 17.221 3.27 24.322 18.322 1.953-14.872-6.86-27.522-12.263-30.83 14.395-43.021.099-41.872-10.808-39.269 9.985 19.97 9.585 27.874 5.687 37.158z' fill='#35890e' filter='url(#B)'/> <path d='M80.361 65.109c4-4.4 8.333-.833 10 1.5-8.799-3.2-18.667 17.667-22.501 28.5 2.5-8.167 8.501-25.6 12.501-30z' fill='#fff' fill-opacity='.4'/> <defs> <filter id='A' x='3.547' y='49.575' width='124.432' height='107.417' filterUnits='userSpaceOnUse' color-interpolation-filters='sRGB' > <feFlood flood-opacity='0'/> <feBlend in='SourceGraphic' result='B'/> <feColorMatrix in='SourceAlpha' values='0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0' result='C'/> <feOffset dx='-2' dy='-4'/> <feGaussianBlur stdDeviation='10'/> <feComposite in2='C' operator='arithmetic' k2='-1' k3='1'/> <feColorMatrix values='0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.6 0'/> <feBlend in2='B' result='D'/> <feColorMatrix in='SourceAlpha' values='0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0' result='C'/> <feOffset dx='5' dy='8'/> <feGaussianBlur stdDeviation='10'/> <feComposite in2='C' operator='arithmetic' k2='-1' k3='1'/> <feColorMatrix values='0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0.25 0'/> <feBlend in2='D'/> </filter> <filter id='B' x='72.86' y='10.442' width='53.784' height='77.152' filterUnits='userSpaceOnUse' color-interpolation-filters='sRGB' > <feFlood flood-opacity='0'/> <feBlend in='SourceGraphic' result='B'/> <feColorMatrix in='SourceAlpha' values='0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0' result='C'/> <feOffset dy='-4'/> <feGaussianBlur stdDeviation='6'/> <feComposite in2='C' operator='arithmetic' k2='-1' k3='1'/> <feColorMatrix values='0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0.45 0'/> <feBlend in2='B' result='D'/> <feColorMatrix in='SourceAlpha' values='0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 0' result='C'/> <feOffset dy='2'/> <feGaussianBlur stdDeviation='3'/> <feComposite in2='C' operator='arithmetic' k2='-1' k3='1'/> <feColorMatrix values='0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0.25 0'/> <feBlend in2='D'/> </filter> </defs> </svg> </div></div></div></div><div class='config-time'> <div class='config-time__header'> <h4>Set Time</h4> </div><div class='config-time__wrapper'> <ul class='config-time__chip-wrapper hide-scrollbar'> <li data-value='10' class='active'>10 <span>Secs</span></li><li data-value='30'>30 <span>Secs</span></li><li data-value='60'>1 <span>Min</span></li><li data-value='180'>3 <span>Min</span></li><li data-value='300'>5 <span>Min</span></li><li data-value='600'>10 <span>Min</span></li><li data-value='1500'>30 <span>Min</span></li><li data-value='3600'>60 <span>Min</span></li></ul> </div></div></div><div class='controls'> <button class='start'>Start Spin</button> <button class='stop hide'>Stop</button> </div></section> </main> </body> <script>const timeChips=document.querySelectorAll( '.config-time__chip-wrapper li' ); const btnStart=document.querySelector('.controls .start'); const btnStop=document.querySelector('.controls .stop'); const notifSpeed=document.querySelector('.notif-speed p'); const notifSpeedSocket=document.querySelector('.notif-speed #speedIn'); const notifTimer=document.querySelector('.notif-timer p'); const notifPower=document.querySelector('.notif-power p'); const notifStatus=document.querySelector('.notif-status p'); const [preloaderPanel, homePanel]=document.querySelectorAll('.panel'); const connectionStatus=document.querySelector( '#home .header .header_network' ); let confetti; const state={isSpinning: false, voltage: '11', spinTime: 10, spinSpeed: 500, tempTime: 0, countdownInterval:{},}; </script> <script>document.addEventListener('DOMContentLoaded', function (){preloaderPanel.addEventListener('animationend', function (){preloaderPanel.classList.add('detach'); homePanel.style.opacity=0; homePanel.classList.remove('detach'); homePanel.style.animationName='fadeIn'}); setTimeout(()=>{preloaderPanel.style.animationName='fadeOut';}, 2000);}); </script> <script>var JSConfetti=(function (){'use strict'; function t(t, e){if (!(t instanceof e)) throw new TypeError('Cannot call a class as a function');}function e(t, e){for (var i=0; i < e.length; i++){var n=e[i]; (n.enumerable=n.enumerable || !1), (n.configurable=!0), 'value' in n && (n.writable=!0), Object.defineProperty(t, n.key, n);}}function i(t, i, n){return i && e(t.prototype, i), n && e(t, n), t;}function n(t){return +t.replace(/px/, '');}function s(t, e){var i=arguments.length > 2 && void 0 !==arguments[2] ? arguments[2] : 0, n=Math.random() * (e - t) + t; return Math.floor(n * Math.pow(10, i)) / Math.pow(10, i);}function o(t){return t[s(0, t.length)];}var a=[ '#fcf403', '#62fc03', '#f4fc03', '#03e7fc', '#03fca5', '#a503fc', '#fc03ad', '#fc03c2',]; function r(t){return Math.log(t) / Math.log(1920);}var h=(function (){function e(i){t(this, e); var n=i.initialPosition, a=i.direction, h=i.confettiRadius, c=i.confettiColors, u=i.emojis, l=i.emojiSize, d=i.canvasWidth, f=s(0.9, 1.7, 3) * r(d); (this.confettiSpeed={x: f, y: f}), (this.finalConfettiSpeedX=s(0.2, 0.6, 3)), (this.rotationSpeed=u.length ? 0.01 : s(0.03, 0.07, 3) * r(d)), (this.dragForceCoefficient=s(5e-4, 9e-4, 6)), (this.radius={x: h, y: h}), (this.initialRadius=h), (this.rotationAngle='left'===a ? s(0, 0.2, 3) : s(-0.2, 0, 3)), (this.emojiSize=l), (this.emojiRotationAngle=s(0, 2 * Math.PI)), (this.radiusYUpdateDirection='down'); var m='left'===a ? (s(82, 15) * Math.PI) / 180 : (s(-15, -82) * Math.PI) / 180; (this.absCos=Math.abs(Math.cos(m))), (this.absSin=Math.abs(Math.sin(m))); var v=s(-150, 0), p={x: n.x + ('left'===a ? -v : v) * this.absCos, y: n.y - v * this.absSin,}; (this.currentPosition=Object.assign({}, p)), (this.initialPosition=Object.assign({}, p)), (this.color=u.length ? null : o(c)), (this.emoji=u.length ? o(u) : null), (this.createdAt=new Date().getTime()), (this.direction=a);}return ( i(e, [{key: 'draw', value: function (t){var e=this.currentPosition, i=this.radius, n=this.color, s=this.emoji, o=this.rotationAngle, a=this.emojiRotationAngle, r=this.emojiSize, h=window.devicePixelRatio; n ? ((t.fillStyle=n), t.beginPath(), t.ellipse( e.x * h, e.y * h, i.x * h, i.y * h, o, 0, 2 * Math.PI ), t.fill()) : s && ((t.font=''.concat(r, 'px serif')), t.save(), t.translate(h * e.x, h * e.y), t.rotate(a), (t.textAlign='center'), t.fillText(s, 0, 0), t.restore());},},{key: 'updatePosition', value: function (t, e){var i=this.confettiSpeed, n=this.dragForceCoefficient, s=this.finalConfettiSpeedX, o=this.radiusYUpdateDirection, a=this.rotationSpeed, r=this.createdAt, h=this.direction, c=e - r; i.x > s && (this.confettiSpeed.x -=n * t), (this.currentPosition.x +=i.x * ('left'===h ? -this.absCos : this.absCos) * t), (this.currentPosition.y=this.initialPosition.y - i.y * this.absSin * c + (0.00125 * Math.pow(c, 2)) / 2), (this.rotationSpeed -=this.emoji ? 1e-4 : 1e-5 * t), this.rotationSpeed < 0 && (this.rotationSpeed=0), this.emoji ? (this.emojiRotationAngle +=(this.rotationSpeed * t) % (2 * Math.PI)) : 'down'===o ? ((this.radius.y -=t * a), this.radius.y <=0 && ((this.radius.y=0), (this.radiusYUpdateDirection='up'))) : ((this.radius.y +=t * a), this.radius.y >=this.initialRadius && ((this.radius.y=this.initialRadius), (this.radiusYUpdateDirection='down')));},},{key: 'getIsVisibleOnCanvas', value: function (t){return this.currentPosition.y < t + 100;},},]), e );})(); function c(){var t=document.createElement('canvas'); return ( (t.style.position='fixed'), (t.style.width='100%'), (t.style.height='100%'), (t.style.top='0'), (t.style.left='0'), (t.style.zIndex='1000'), (t.style.pointerEvents='none'), document.body.appendChild(t), t );}function u(t){var e=t.confettiRadius, i=void 0===e ? 6 : e, n=t.confettiNumber, s=void 0===n ? t.confettiesNumber || (t.emojis ? 40 : 250) : n, o=t.confettiColors, r=void 0===o ? a : o, h=t.emojis, c=void 0===h ? t.emojies || [] : h, u=t.emojiSize, l=void 0===u ? 80 : u; return ( t.emojies && console.error( 'emojies argument is deprecated, please use emojis instead' ), t.confettiesNumber && console.error( 'confettiesNumber argument is deprecated, please use confettiNumber instead' ),{confettiRadius: i, confettiNumber: s, confettiColors: r, emojis: c, emojiSize: l,});}var l=(function (){function e(i){var n=this; t(this, e), (this.canvasContext=i), (this.shapes=[]), (this.promise=new Promise(function (t){return (n.resolvePromise=t);}));}return ( i(e, [{key: 'getBatchCompletePromise', value: function (){return this.promise;},},{key: 'addShapes', value: function (){var t; (t=this.shapes).push.apply(t, arguments);},},{key: 'complete', value: function (){var t; return ( !this.shapes.length && (null===(t=this.resolvePromise) || void 0===t || t.call(this), !0) );},},{key: 'processShapes', value: function (t, e, i){var n=this, s=t.timeDelta, o=t.currentTime; this.shapes=this.shapes.filter(function (t){return ( t.updatePosition(s, o), t.draw(n.canvasContext), !i || t.getIsVisibleOnCanvas(e) );});},},]), e );})(); return (function (){function e(){var i=arguments.length > 0 && void 0 !==arguments[0] ? arguments[0] :{}; t(this, e), (this.activeConfettiBatches=[]), (this.canvas=i.canvas || c()), (this.canvasContext=this.canvas.getContext('2d')), (this.requestAnimationFrameRequested=!1), (this.lastUpdated=new Date().getTime()), (this.iterationIndex=0), (this.loop=this.loop.bind(this)), requestAnimationFrame(this.loop);}return ( i(e, [{key: 'loop', value: function (){var t, e, i, s, o; (this.requestAnimationFrameRequested=!1), (t=this.canvas), (e=window.devicePixelRatio), (i=getComputedStyle(t)), (s=n(i.getPropertyValue('width'))), (o=n(i.getPropertyValue('height'))), t.setAttribute('width', (s * e).toString()), t.setAttribute('height', (o * e).toString()); var a=new Date().getTime(), r=a - this.lastUpdated, h=this.canvas.offsetHeight, c=this.iterationIndex % 10==0; (this.activeConfettiBatches=this.activeConfettiBatches.filter( function (t){return ( t.processShapes({timeDelta: r, currentTime: a}, h, c), !c || !t.complete() );})), this.iterationIndex++, this.queueAnimationFrameIfNeeded(a);},},{key: 'queueAnimationFrameIfNeeded', value: function (t){this.requestAnimationFrameRequested || this.activeConfettiBatches.length < 1 || ((this.requestAnimationFrameRequested=!0), (this.lastUpdated=t || new Date().getTime()), requestAnimationFrame(this.loop));},},{key: 'addConfetti', value: function (){for ( var t=arguments.length > 0 && void 0 !==arguments[0] ? arguments[0] :{}, e=u(t), i=e.confettiRadius, n=e.confettiNumber, s=e.confettiColors, o=e.emojis, a=e.emojiSize, r=this.canvas.getBoundingClientRect(), c=r.width, d=r.height, f=(5 * d) / 7, m={x: 0, y: f}, v={x: c, y: f}, p=new l(this.canvasContext), g=0; g < n / 2; g++ ){var y=new h({initialPosition: m, direction: 'right', confettiRadius: i, confettiColors: s, confettiNumber: n, emojis: o, emojiSize: a, canvasWidth: c,}), C=new h({initialPosition: v, direction: 'left', confettiRadius: i, confettiColors: s, confettiNumber: n, emojis: o, emojiSize: a, canvasWidth: c,}); p.addShapes(y, C);}return ( this.activeConfettiBatches.push(p), this.queueAnimationFrameIfNeeded(), p.getBatchCompletePromise() );},},{key: 'clearCanvas', value: function (){this.activeConfettiBatches=[];},},]), e );})();})(); confetti=new JSConfetti(); </script> <script>function map(value, fromMin, fromMax, toMin, toMax, interval){if (value < fromMin){value=fromMin;}else if (value > fromMax){value=fromMax;}const index=(value - fromMin) / (fromMax - fromMin); const mappedValue=Math.floor((index * (toMax - toMin + interval)) / interval) * interval + toMin; return mappedValue;}function secondsToMinutes(seconds){const minutes=Math.floor(seconds / 60); const remainingSeconds=seconds % 60; return `${minutes.toString().padStart(2, '0')}:${remainingSeconds .toString() .padStart(2, '0')}`;}</script> <script>let countdownInterval; let remainingTime=0; let centrifugeSpeed=1500; let centrifugeSpeed1=1500; let Socket; const init=()=>{Socket=new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage=function (event){processCommand(event);}; handleWebSocketEvents();}; function handleWebSocketEvents(){Socket.addEventListener('open', ()=>{updateWebSocketStatusClass();}); Socket.addEventListener('close', ()=>{updateWebSocketStatusClass();}); Socket.addEventListener('error', ()=>{updateWebSocketStatusClass();});}function updateWebSocketStatusClass(){if (Socket.readyState===WebSocket.OPEN){connectionStatus.classList.remove('disconnected'); connectionStatus.classList.add('connected');}else{connectionStatus.classList.remove('connected'); connectionStatus.classList.add('disconnected');}}function processCommand(event){var obj=JSON.parse(event.data); notifPower.textContent=obj.rand1; if (state.isSpinning){notifSpeedSocket.textContent=obj.rand2;}}function toggleOptionsVisibility(isVisible){const optionsElements=document.querySelectorAll('input'); if (!isVisible){timeChips.forEach((timeChip)=>{timeChip.classList.add('disabled');});}else{timeChips.forEach((timeChip)=>{timeChip.classList.remove('disabled');});}optionsElements.forEach((element)=>{element.disabled=!isVisible;});}function updateTimer(){const minutes=Math.floor(state.tempTime / 60); const seconds=state.tempTime % 60; const timerDisplay=`${minutes.toString().padStart(2, '0')}:${seconds < 10 ? '0' : ''}${seconds}`; notifTimer.textContent=timerDisplay; if (state.tempTime <=10){notifTimer.classList.add('blink');}if (state.tempTime <=0){confetti.addConfetti({emojis: ['🌈', '⚡️', '💥', '✨', '💫', '🌸'],}); clearInterval(countdownInterval); notifStatus.textContent='Idle'; notifTimer.textContent=secondsToMinutes(state.spinTime); toggleOptionsVisibility(true); notifTimer.classList.remove('blink'); state.isSpinning=false; notifSpeedSocket.classList.add('hide'); btnStop.classList.add('hide'); btnStart.classList.remove('hide');}else{state.tempTime--;}}function updateSpeed(){notifSpeed.textContent=`${centrifugeSpeed}`;}window.onload=function (event){init();}; </script> <script>timeChips.forEach((timeChip)=>{timeChip.addEventListener('click', (e)=>{timeChips.forEach((timeChip)=>{timeChip.classList.remove('active');}); timeChip.classList.add('active'); state.spinTime=parseInt(timeChip.dataset.value); notifTimer.textContent=secondsToMinutes(timeChip.dataset.value);});}); btnStart.addEventListener('click', (e)=>{state.isSpinning=true; state.tempTime=state.spinTime; notifSpeedSocket.classList.remove('hide'); btnStop.classList.remove('hide'); btnStart.classList.add('hide'); notifStatus.textContent='Active'; toggleOptionsVisibility(false); countdownInterval=setInterval(updateTimer, 1000); let msg={brand: map(parseInt(state.spinSpeed), 500, 6000, 1100, 1800, 50), type: parseInt(state.spinTime), color: 'Start',}; console.log(msg); Socket.send(JSON.stringify(msg));}); btnStop.addEventListener('click', (e)=>{state.isSpinning=false; state.tempTime=0; notifSpeedSocket.classList.add('hide'); btnStart.classList.remove('hide'); btnStop.classList.add('hide'); toggleOptionsVisibility(true); clearInterval(countdownInterval); notifTimer.textContent=secondsToMinutes(state.spinTime); notifTimer.classList.remove('blink'); let msg={brand: 1000, type: 0, color: 'Stop'}; Socket.send(JSON.stringify(msg));}); </script> <script>const sliderWrapper=document.querySelector('.slider-wrapper'); const sliderInput=document.querySelector('.slider-input'); const sliderTrack=document.querySelector('.slider-track'); const sliderThumb=document.querySelector('.slider-thumb'); const sliderThumbImage=document.querySelector('.slider-thumb-image'); const minValue=+sliderInput.min || 0; const maxValue=+sliderInput.max || 100; const colorStops=[{r: 98, g: 173, b: 29},{r: 251, g: 189, b: 30},{r: 251, g: 110, b: 30},{r: 231, g: 26, b: 26},]; const setSliderThumbImageColor=(progress)=>{const index=((colorStops.length - 1) * progress) / 100; const startIndex=Math.floor(index); const endIndex=Math.ceil(index); const startColor=colorStops[startIndex]; const endColor=colorStops[endIndex]; const percentage=index - startIndex; const [r, g, b]=[ Math.round(startColor.r + (endColor.r - startColor.r) * percentage), Math.round(startColor.g + (endColor.g - startColor.g) * percentage), Math.round(startColor.b + (endColor.b - startColor.b) * percentage),]; sliderThumbImage.style.setProperty( '--slider-thumb-image-color', `rgb(${r}${g}${b})` );}; const updateSlider=()=>{const progress=(100 * +sliderInput.value) / (maxValue - minValue); const mappedProgress=map(progress, 0, 100, 500, 6000, 50); state.spinSpeed=mappedProgress; notifSpeed.textContent=mappedProgress; sliderWrapper.style.setProperty('--slider-progress', progress); setSliderThumbImageColor(progress); console.log(state);}; sliderInput.addEventListener('input', updateSlider); updateSlider(); let pointerHoldingRightTimer; let isTimerRunning=false; const detectPointerHoldingRight=(event)=>{const isSliderAtMax=+sliderInput.value===maxValue; const sliderRect=sliderWrapper.getBoundingClientRect(); const pointerX=event.clientX; const offset=40; const isPointerOnSliderRight=pointerX >=sliderRect.right + offset; if (isSliderAtMax && isPointerOnSliderRight){if (!isTimerRunning){isTimerRunning=true; sliderThumbImage.classList.add('shake'); pointerHoldingRightTimer=setTimeout(()=>{sliderInput.removeEventListener('pointerdown', detectSecret); colorStops.push({r: 34, g: 0, b: 0}); sliderWrapper.classList.add('unlocked'); sliderThumb.addEventListener('animationend', ()=>{sliderThumb.classList.remove('play-explosion');}); sliderThumb.classList.add('play-explosion'); setSliderThumbImageColor(100); sliderThumbImage.classList.remove('shake');}, 1000);}}else{clearTimeout(pointerHoldingRightTimer); isTimerRunning=false; sliderThumbImage.classList.remove('shake');}}; const detectSecret=()=>{window.addEventListener('pointermove', detectPointerHoldingRight); sliderInput.addEventListener('pointerup', ()=>{window.removeEventListener('pointermove', detectPointerHoldingRight); clearTimeout(pointerHoldingRightTimer); sliderThumbImage.classList.remove('shake');});}; sliderInput.addEventListener('pointerdown', detectSecret); </script></html>";

int interval = 1000;
unsigned long previousMillis = 0;
int speed1 = 0;
int sevpin = 17; //servo
int batPin = 32;
int buzz = 15;
int greenLed = 4;
int redLed = 16;
int Sec = 0;
int bcount = 2;

bool buzzer = true;

Servo channel_1;

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81); 

void setup() {
  pinMode(batPin, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  Serial.begin(115200); // init serial port for debugging
  channel_1.attach(sevpin);
  channel_1.writeMicroseconds(1000);
  delay(1000);
  channel_1.writeMicroseconds(2000);
  delay(1000);
  channel_1.writeMicroseconds(1000); // WiFi.begin(ssid, password);                         // start WiFi interface

  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(Actual_IP);

  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP()); // show IP address that the ESP32 has received from router

  server.on("/", []() { // define here wat the webserver needs to do
    server.send(200, "text/html", webpage); //    -> it needs to send out the HTML string "webpage" to the client
  });
  server.begin(); // start server

  webSocket.begin(); // start websocket
  webSocket.onEvent(webSocketEvent);

  // Start mDNS. Basic error check.
  if (!MDNS.begin("bigfuge")) {
    Serial.println("Error starting mDNS");
    return;
  }

  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
}

void loop() {
  server.handleClient(); // Needed for the webserver to handle all clients
  webSocket.loop(); // Update function for the webSockets

  unsigned long now = millis(); // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if ((unsigned long)(now - previousMillis) > interval) { // check if "interval" ms has passed since last time the clients were updated

    String jsonString = ""; // create a JSON string for sending data to the client
    StaticJsonDocument < 200 > doc; // create a JSON container
    JsonObject object = doc.to < JsonObject > (); // create a JSON Object
    object["rand1"] = String(battery_volt(), 2) + "V"; //battery percentage;                    // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
    object["rand2"] = calSpeed(); //speed;
    serializeJson(doc, jsonString); // convert JSON object to string
    Serial.println(jsonString); // print JSON string to console for debug purposes (you can comment this out)
    webSocket.broadcastTXT(jsonString); // send JSON string to clients
    Serial.print(Sec);
    if (Sec > 0) {
      --Sec;
      bcount = 1;
    } else {
      Serial.println("Done");
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      speed1 = 0;
      channel_1.writeMicroseconds(1000);
      if (bcount > 0) {
        digitalWrite(buzz, HIGH);
        Serial.println("Buzzing...");
        bcount++;
        if (bcount > 3) {
          digitalWrite(buzz, LOW);
          Serial.println("Done Buzzing");
          bcount = 0;

        }

      }
    }
    previousMillis = now; // reset previousMillis
  }
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) { // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
  switch (type) { // switch on the type of information sent
  case WStype_DISCONNECTED: // if a client is disconnected, then type == WStype_DISCONNECTED
    Serial.println("Client " + String(num) + " disconnected");
    break;
  case WStype_CONNECTED: // if a client is connected, then type == WStype_CONNECTED
    Serial.println("Client " + String(num) + " connected");
    // optionally you can add code here what to do when connected
    break;
  case WStype_TEXT: // if a client has sent data, then type == WStype_TEXT
    // try to decipher the JSON string received
    StaticJsonDocument < 200 > doc; // create a JSON container
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    } else {
      // JSON string was received correctly, so information can be retrieved:
      const int g_brand = doc["brand"];
      const int g_type = doc["type"];
      const int g_year = doc["year"];
      const String g_color = doc["color"];
      channel_1.writeMicroseconds(g_brand);
      Sec = g_type;
      //        speed1 = g_brand;
      if (g_color == "Start") {
        if (buzzer) {
          beep_buzz(2);
          buzzer = false;
        }
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
        speed1 = g_brand;
      } else {
        buzzer = true;
        speed1 = 0;
        Serial.println("Stopping");
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, LOW);
        channel_1.writeMicroseconds(g_brand);
        Sec = g_type;
      }

    }
    Serial.println("");
    break;
  }
}

float battery_volt() {

  int batArrSize = 30;
  int batArr[batArrSize] = {
    0
  };
  for (int q = 0; q < batArrSize; q++) {
    batArr[q] = analogRead(batPin);
    //    Serial.println("Battery ADC: ");
    //    Serial.println(batArr[q]);
  }
  int batADC = max_frequency(batArr, batArrSize);

  //  batteryADC = batADC;
  float batVolt = 0.00;
  batVolt = ((float) batADC * 3.3) / ((float) 4095);
  batVolt = batVolt * (1 / 0.1754); // --> 1/(R2/(R1+R2)    correct 1024 to 1023
  if (batVolt <= 0.2) {
    batVolt = 0;
  } else {
    batVolt += 0.7;
  }

  Serial.println("Battery Voltage: ");
  Serial.println(batVolt);

  return decimal_round(batVolt, 2);
}

void beep_buzz(int num) {
  for (int i = 0; i < num; i++) {
    digitalWrite(buzz, HIGH);
    delay(50);
    digitalWrite(buzz, LOW);
    delay(50);
  }
}

float decimal_round(float num, int decimals) {
  float scale = pow(10, decimals);
  return round(num * scale) / scale;
}

int calSpeed() {
  float s = float(speed1) / 2000;
  s = s * battery_volt() * 930;
  return int(s);
}

//int calSpeed() {
//  int s = speed1;
//  s = s * battery_volt();
//  return 0;
//
//}

/////////////////////////////////////////////////////////////////////////////////////
int max_frequency(int sampleArray[], int arraySize) {
  for (int j = 0; j < arraySize; j++) {
    //    Serial.print("SAMPLE ARRAY: ");
    //    Serial.print(sampleArray[j]);
    //    Serial.print("\n");
  }

  int freqArray[2][arraySize];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < arraySize; j++) {
      freqArray[i][j] = -1;
    }
  }
  bool check[arraySize];
  for (int i = 0; i < arraySize; i++) {
    check[i] = 0;
  }

  int indexCount = 1;
  for (int i = 0; i < arraySize; i++) {
    if (check[i] == 1) {
      continue;
    }
    int freqCount = 1;
    for (int j = i + 1; j < arraySize; j++) {
      if (sampleArray[i] == sampleArray[j]) {
        check[j] = 1;
        freqCount++;
      }
    }

    freqArray[0][indexCount] = freqCount;
    freqArray[1][indexCount] = sampleArray[i];
    indexCount++;
  }

  //MAXIMUM FREQUENCY CALCULATION SECTOR
  int maxFreq = freqArray[0][0];
  int maxIndex;

  for (int j = 0; j < indexCount + 1; j++) {
    if (freqArray[0][j] > maxFreq) {
      maxFreq = freqArray[0][j];
      maxIndex = j;
    }
  }

  int sample = freqArray[1][maxIndex];
  return sample;
}
