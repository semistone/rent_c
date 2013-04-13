// Include our 'js-yaml' module.
var yaml = require("js-yaml");

/* Import the 'config.yaml' file in the current directory, convert it to JSON, and store it in the `config` variable. */
var config = require("./config.yaml");

/* Pretty-print the JSON object out to the stdout/console. */
console.log(JSON.stringify(config, null, "    "));
