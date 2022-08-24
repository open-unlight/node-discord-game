// Type definitions for Discord Game
// Project: Open Unlight
// Definitions by: Aotokitsuruya <https://blog.frost.tw>

/**
 * Discord Main Function
 */
export as namespace Discord;

export const version: number;
export function create(appId: string, isRequireDiscord: boolean): boolean;
export function runCallback(): boolean;

/**
 * Application
 */

export namespace Application {
  export const locale: string;
  export const branch: string;
  export function getOAuth2Token(): Promise<string>;
}

/**
 * Activity
 */

export namespace Activity {
  export interface ActivityAssets {
    largeImage?: string;
    largeText?: string;
    smallImage?: string;
    smallText?: string
  }

  export interface ActivityTimestamps {
    startAt?: Date;
    endAt?: Date;
  }

  export interface ActivitySecrets {
    match?: string;
    join?: string;
    spectate?: string;
  }

  export interface ActivityParty {
    id?: string;
    currentSize?: number;
    maxSize?: number;
  }

  export interface Activity {
    state?: string;
    details?: string;
    assets?: ActivityAssets;
    timestamps?: ActivityTimestamps;
    secrets?: ActivitySecrets;
    party?: ActivityParty;
  }
  export function update(activity: Activity): Promise<boolean>;
  export function clear(): Promise<boolean>;
}
