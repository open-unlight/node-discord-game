const assert = require('assert');
const Discord = require('../');

describe('Discord', () => {
  describe('version', () => {
    it('should be 2', () => {
      assert.equal(2, Discord.version);
    });
  });
});
