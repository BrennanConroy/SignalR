const path = require("path");

let defaultReporters = ["progress", "summary"];

/** Creates the Karma config function based on the provided options
 *
 * @param {object} config Configuration options to override on/add to the base config.
 */
function createKarmaConfig(config) {
    return function (karmaConfig) {
        karmaConfig.set({
            basePath: path.resolve(__dirname, ".."),
            frameworks: ["jasmine"],
            files: [
                "wwwroot/lib/msgpack5/msgpack5.js",
                "node_modules/@aspnet/signalr/dist/browser/signalr.js",
                "node_modules/@aspnet/signalr-protocol-msgpack/dist/browser/signalr-protocol-msgpack.js",
                "wwwroot/dist/signalr-functional-tests.js"
            ],
            preprocessors: {
                "**/*.js": ["sourcemap"]
            },
            port: 9876,
            colors: true,
            logLevel: config.LOG_INFO,
            autoWatch: false,
            singleRun: false,
            concurrency: Infinity,

            // Log browser messages to a file, not the terminal.
            browserConsoleLogOptions: {
                level: "debug",
                terminal: false
            },

            // Increase some timeouts that are a little aggressive when multiple browsers (or SauceLabs) are in play.
            browserDisconnectTimeout: 10000, // default 2000
            browserDisconnectTolerance: 1, // default 0
            browserNoActivityTimeout: 4 * 60 * 1000, //default 10000
            captureTimeout: 4 * 60 * 1000, //default 60000

            // Override/add values using the passed-in config.
            ...config,

            // Apply the default reporters along with whatever was passed in
            reporters: [...defaultReporters, ...(config.reporters || [])],
        });
    }
}

module.exports = createKarmaConfig;