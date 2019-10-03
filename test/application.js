const assert = require('assert');
const Discord = require('../');

describe('Application', () => {
  describe('branch', () => {
    it('should be false', () => {
      assert.equal(false, Discord.Application.branch);
    });

    // TODO: Test with Discord created status
  });

  describe('locale', () => {
    it('should be false', () => {
      assert.equal(false, Discord.Application.locale);
    });

    // TODO: Test with Discord created status
  });
});
