Discord Game SDK
===

This is node.js module for Electron to integrate with Discord.

## Requirements

* Node.js 11+
* Electron 6.0+

## Usage

Before using this module, you have to go to [Discord Developer](https://discordapp.com/developers/applications/) to create an application.

### Discord

#### create()

Initialize Discord.

```js
const Discord = require('discord_game');

const isRequireDiscord = true;
Discord.create('Client ID', isRequireDiscord);
// => true
```

### version

Get Discord Game SDK Version

### runCallback()

Resolve Discord's async behavior and call the callback;

```js
const Discord = require('discord_game');

const isRequireDiscord = true;
Discord.create('Client ID', isRequireDiscord);

setInterval(function() {
  Discord.runCallback(); // => true
}, 1000/60)
```

### Application

#### locale

Get the current locale

#### branch

Get the current branch

#### getOAuth2Token()

Get the user OAuth2 Token.

```js
const Discord = require('discord_game');

const isRequireDiscord = true;
Discord.create('Client ID', isRequireDiscord);

Discord.Application
       .getOAuth2Token()
       .then(function(token) { console.log('Token is', token) });

setInterval(function() {
  Discord.runCallback(); // => true
}, 1000/60)
```

### Activity (aka Rich Presence)

#### update

Update user's rich presence.

> Not fully support

```js
const Discord = require('discord_game');

const isRequireDiscord = true;
Discord.create('Client ID', isRequireDiscord);

const start_at = new Date();
Discord.Activity
       .update('Detail', 'State', 'Large Image', start_at)
       .then(function() { console.log('Rich Presence updated') });

setInterval(function() {
  Discord.runCallback(); // => true
}, 1000/60)
```
